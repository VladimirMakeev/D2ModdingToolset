/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "netcustomservice.h"
#include "lobbyclient.h"
#include "log.h"
#include "mempool.h"
#include "midgard.h"
#include "mqnetservice.h"
#include "netcustomsession.h"
#include "settings.h"
#include "utils.h"
#include <MessageIdentifiers.h>
#include <fmt/format.h>
#include <mutex>

namespace hooks {

CNetCustomService::CNetCustomService(std::unique_ptr<SLNet::Lobby2Client>&& lobbyClient,
                                     std::unique_ptr<SLNet::Lobby2MessageFactory>&& msgFactory,
                                     std::unique_ptr<LoggingCallbacks>&& logCallbacks,
                                     std::unique_ptr<SLNet::RoomsPlugin>&& roomsClient,
                                     std::unique_ptr<RoomsLoggingCallback>&& roomsCallback,
                                     SLNet::RakPeerInterface* peer)
    : lobbyClient(std::move(lobbyClient))
    , lobbyMsgFactory(std::move(msgFactory))
    , loggingCallbacks(std::move(logCallbacks))
    , roomsClient(std::move(roomsClient))
    , roomsLogCallback(std::move(roomsCallback))
    , lobbyPeer(peer)
{ }

CNetCustomService* getNetService()
{
    using namespace game;

    auto midgard = CMidgardApi::get().instance();
    auto service = midgard->data->netService;
    if (!service) {

        return nullptr;
    }

    return static_cast<CNetCustomService*>(service);
}

void __fastcall lobbyPacketEventCallback(CNetCustomService* thisptr, int /*%edx*/)
{
    if (!thisptr->lobbyPeer || !thisptr->lobbyClient || !thisptr->lobbyMsgFactory) {
        return;
    }

    static std::mutex packetMutex;
    std::lock_guard<std::mutex> guard(packetMutex);

    auto peer{thisptr->lobbyPeer};

    for (auto packet = peer->Receive(); packet != nullptr;
         peer->DeallocatePacket(packet), packet = peer->Receive()) {
        switch (packet->data[0]) {
        case ID_DISCONNECTION_NOTIFICATION:
            logDebug("lobby.log", "Disconnected");
            // thisptr->lobbyClient->RemoveCallbackInterface(clientCallbacks.get());
            break;
        case ID_ALREADY_CONNECTED:
            logDebug("lobby.log", "Already connected");
            break;
        case ID_CONNECTION_LOST:
            logDebug("lobby.log", "Connection lost");
            // lobbyClient->RemoveCallbackInterface(clientCallbacks.get());
            break;
        case ID_CONNECTION_ATTEMPT_FAILED:
            logDebug("lobby.log", "Connection attempt failed");
            // lobbyClient->RemoveCallbackInterface(clientCallbacks.get());
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            logDebug("lobby.log", "Server is full");
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED: {
            logDebug("lobby.log", "Connection request accepted, set server address");
            // Make sure plugins know about the server
            thisptr->lobbyClient->SetServerAddress(packet->systemAddress);
            thisptr->roomsClient->SetServerAddress(packet->systemAddress);

            // clientCallbacks = std::make_unique<ClientCallbacks>(this);
            // lobbyClient->AddCallbackInterface(clientCallbacks.get());
            break;
        }
        case ID_LOBBY2_SERVER_ERROR:
            logDebug("lobby.log", "Lobby server error");
            break;
        default:
            logDebug("lobby.log",
                     fmt::format("Packet type {:d}", static_cast<int>(packet->data[0])));
            break;
        }
    }
}

void __fastcall netCustomServiceDtor(CNetCustomService* thisptr, int /*%edx*/, char flags)
{
    logDebug("lobby.log", "CNetCustomService d-tor called");

    thisptr->lobbyPeer->Shutdown(1000);

    logDebug("lobby.log", "Remove packet processing event");
    game::UiEventApi::get().destructor(&thisptr->lobbyPacketEvent);

    logDebug("lobby.log", "Destroy lobby instances");
    SLNet::RakPeerInterface::DestroyInstance(thisptr->lobbyPeer);
    thisptr->roomsLogCallback.reset(nullptr);
    thisptr->roomsClient.reset(nullptr);
    thisptr->loggingCallbacks.reset(nullptr);
    thisptr->lobbyMsgFactory.reset(nullptr);
    thisptr->lobbyClient.reset(nullptr);
    thisptr->loggedAccount.~basic_string();

    if (flags & 1) {
        logDebug("lobby.log", "CNetCustomService d-tor frees memory");
        game::Memory::get().freeNonZero(thisptr);
    }
}

bool __fastcall netCustomServiceHasSessions(CNetCustomService* thisptr, int /*%edx*/)
{
    logDebug("lobby.log", "CNetCustomService hasSessions called");
    return false;
}

void __fastcall netCustomServiceGetSessions(CNetCustomService* thisptr,
                                            int /*%edx*/,
                                            game::LinkedList<game::IMqNetSessEnum*>* sessions,
                                            const GUID* appGuid,
                                            const char* ipAddress,
                                            bool allSessions,
                                            bool requirePassword)
{
    // This method used by vanilla interface.
    // Since we have our custom one, we can ignore it and there is no need to implement.
    logDebug("lobby.log", "CNetCustomService getSessions called");
}

void __fastcall netCustomServiceCreateSession(CNetCustomService* thisptr,
                                              int /*%edx*/,
                                              game::IMqNetSession** netSession,
                                              const GUID* /* appGuid */,
                                              const char* sessionName,
                                              const char* password)
{
    logDebug("lobby.log",
             fmt::format("CNetCustomService createSession called. Name '{:s}'", sessionName));

    *netSession = nullptr;

    if (tryCreateRoom(sessionName)) {
        // Host is the one who creates the session. DirectPlay implementation does the same.
        *netSession = createCustomNetSession(thisptr, sessionName, true);
    }
}

void __fastcall netCustomServiceJoinSession(CNetCustomService* thisptr,
                                            int /*%edx*/,
                                            game::IMqNetSession** netSession,
                                            game::IMqNetSessEnum* netSessionEnum,
                                            const char* password)
{
    // This method used by vanilla interface.
    // Since we are using our custom one, we can join session directly and ignore this method.
    logDebug("lobby.log", "CNetCustomService joinSession called");
}

static game::IMqNetServiceVftable netCustomServiceVftable{
    (game::IMqNetServiceVftable::Destructor)netCustomServiceDtor,
    (game::IMqNetServiceVftable::HasSessions)netCustomServiceHasSessions,
    (game::IMqNetServiceVftable::GetSessions)netCustomServiceGetSessions,
    (game::IMqNetServiceVftable::CreateSession)netCustomServiceCreateSession,
    (game::IMqNetServiceVftable::JoinSession)netCustomServiceJoinSession,
};

bool createCustomNetService(game::IMqNetService** service)
{
    using namespace game;

    *service = nullptr;

    logDebug("lobby.log", "Get peer instance");
    auto lobbyPeer = SLNet::RakPeerInterface::GetInstance();

    const auto& lobbySettings = userSettings().lobby;
    const auto& clientPort = lobbySettings.client.port;
    SLNet::SocketDescriptor socket{clientPort, nullptr};

    logDebug("lobby.log", fmt::format("Start lobby peer on port {:d}", clientPort));

    if (lobbyPeer->Startup(1, &socket, 1) != SLNet::RAKNET_STARTED) {
        logError("lobby.log", "Failed to start lobby client");
        SLNet::RakPeerInterface::DestroyInstance(lobbyPeer);
        return false;
    }

    const auto& serverIp = lobbySettings.server.ip;
    const auto& serverPort = lobbySettings.server.port;

    logDebug("lobby.log", fmt::format("Connecting to lobby server with ip '{:s}', port {:d}",
                                      serverIp, serverPort));

    if (lobbyPeer->Connect(serverIp.c_str(), serverPort, nullptr, 0)
        != SLNet::CONNECTION_ATTEMPT_STARTED) {
        logError("lobby.log", "Failed to connect to lobby server");
        SLNet::RakPeerInterface::DestroyInstance(lobbyPeer);
        return false;
    }

    logDebug("lobby.log", "Create client");
    auto lobbyClient = std::make_unique<SLNet::Lobby2Client>();

    logDebug("lobby.log", "Create msg factory");
    auto lobbyMsgFactory = std::make_unique<SLNet::Lobby2MessageFactory>();

    logDebug("lobby.log", "Set msg factory");
    lobbyClient->SetMessageFactory(lobbyMsgFactory.get());

    logDebug("lobby.log", "Create callbacks");
    auto callbacks = std::make_unique<LoggingCallbacks>();
    lobbyClient->SetCallbackInterface(callbacks.get());

    logDebug("lobby.log", "Attach lobby client as a plugin");
    lobbyPeer->AttachPlugin(lobbyClient.get());

    auto roomsClient = std::make_unique<SLNet::RoomsPlugin>();
    lobbyPeer->AttachPlugin(roomsClient.get());

    auto roomsCallback = std::make_unique<RoomsLoggingCallback>();
    roomsClient->SetRoomsCallback(roomsCallback.get());

    logDebug("lobby.log", "Allocate CNetCustomService");
    auto netService = (CNetCustomService*)game::Memory::get().allocate(sizeof(CNetCustomService));

    logDebug("lobby.log", "Call placement new");

    new (netService)
        CNetCustomService(std::move(lobbyClient), std::move(lobbyMsgFactory), std::move(callbacks),
                          std::move(roomsClient), std::move(roomsCallback), lobbyPeer);

    logDebug("lobby.log", "Assign vftable");
    netService->vftable = &netCustomServiceVftable;

    logDebug("lobby.log", "Create lobby client packet event");
    createTimerEvent(&netService->lobbyPacketEvent, netService, lobbyPacketEventCallback, 100);

    logDebug("lobby.log", "CNetCustomService created");
    *service = netService;
    return true;
}

void addLobbyCallbacks(SLNet::Lobby2Callbacks* callbacks)
{
    auto netService{getNetService()};
    if (!netService) {
        return;
    }

    netService->lobbyClient->AddCallbackInterface(callbacks);
}

void removeLobbyCallbacks(SLNet::Lobby2Callbacks* callbacks)
{
    auto netService{getNetService()};
    if (!netService) {
        return;
    }

    netService->lobbyClient->RemoveCallbackInterface(callbacks);
}

void addRoomsCallback(SLNet::RoomsCallback* callback)
{
    auto netService{getNetService()};
    if (!netService) {
        return;
    }

    logDebug("lobby.log", fmt::format("Adding room callback {:p}", (void*)callback));
    netService->roomsClient->AddRoomsCallback(callback);
}

void removeRoomsCallback(SLNet::RoomsCallback* callback)
{
    auto netService{getNetService()};
    if (!netService) {
        return;
    }

    logDebug("lobby.log", fmt::format("Removing room callback {:p}", (void*)callback));
    netService->roomsClient->RemoveRoomsCallback(callback);
}

} // namespace hooks
