/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#include "roomservercreation.h"
#include "dialoginterf.h"
#include "editboxinterf.h"
#include "lobbyclient.h"
#include "log.h"
#include "mempool.h"
#include "menubase.h"
#include "menuflashwait.h"
#include "netcustomplayerclient.h"
#include "netcustomplayerserver.h"
#include "netcustomservice.h"
#include "netcustomsession.h"
#include "originalfunctions.h"
#include "roomscallback.h"
#include "settings.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

game::CMenuFlashWait* menuWaitServer{nullptr};
game::CMenuBase* menuBase{nullptr};

static void hideWaitMenu()
{
    if (menuWaitServer) {
        hideInterface(menuWaitServer);
        menuWaitServer->vftable->destructor(menuWaitServer, 1);
        menuWaitServer = nullptr;
    }
}

static void serverCreationError(const std::string& message)
{
    hideWaitMenu();
    showMessageBox(message);
}

/**
 * Handles host player client connection to player server.
 * Sets up host player net ids and executes original game logic from CMenuNewSkirmish BTN_LOAD.
 */
class HostClientConnectCallbacks : public NetworkPeerCallbacks
{
public:
    HostClientConnectCallbacks() = default;
    ~HostClientConnectCallbacks() override = default;

    void onPacketReceived(DefaultMessageIDTypes type,
                          SLNet::RakPeerInterface* peer,
                          const SLNet::Packet* packet) override
    {
        auto service = getNetService();
        auto hostPlayer = service->session->players.front();

        if (type == ID_CONNECTION_ATTEMPT_FAILED) {
            serverCreationError("Host player failed to connect to player server");
            // Unsubscribe from callbacks
            hostPlayer->player.netPeer.removeCallback(this);
            return;
        }

        if (type != ID_CONNECTION_REQUEST_ACCEPTED) {
            return;
        }

        // Unsubscribe from callbacks
        hostPlayer->player.netPeer.removeCallback(this);
        hideWaitMenu();

        // Setup host player netId and remember serverId
        auto& player = hostPlayer->player;
        player.netId = SLNet::RakNetGUID::ToUint32(peer->GetMyGUID());

        auto serverGuid = peer->GetGuidFromSystemAddress(packet->systemAddress);
        hostPlayer->serverId = SLNet::RakNetGUID::ToUint32(serverGuid);

        hostPlayer->setupPacketCallbacks();

        logDebug("lobby.log",
                 fmt::format("Host player netId 0x{:x} connected to player server netId 0x{:x}",
                             player.netId, hostPlayer->serverId));

        if (menuBase) {
            logDebug("lobby.log", "Proceed to next screen");
            getOriginalFunctions().menuNewSkirmishLoadScenario(menuBase);
        } else {
            logDebug("lobby.log", "MenuBase is null somehow!");
        }
    }
};

static HostClientConnectCallbacks hostClientConnectCallbacks;

static void createHostPlayer()
{
    auto service = getNetService();

    logDebug("lobby.log", "Creating host player client");

    // Connect player client to local player server, wait response.
    // Use dummy name, it will be set properly later in session::CreateClient method
    auto hostPlayer = createCustomHostPlayerClient(service->session, "Host player");
    service->session->players.push_back(hostPlayer);

    logDebug("lobby.log", "Host player client waits for player server connection response");

    hostPlayer->player.netPeer.addCallback(&hostClientConnectCallbacks);
}

/**
 * Handles player server connection to lobby server.
 * Attaches NAT client and creates host player on successfull connection.
 */
class ServerConnectCallbacks : public NetworkPeerCallbacks
{
public:
    ServerConnectCallbacks() = default;
    ~ServerConnectCallbacks() override = default;

    void onPacketReceived(DefaultMessageIDTypes type,
                          SLNet::RakPeerInterface* peer,
                          const SLNet::Packet* packet) override
    {
        auto service = getNetService();
        auto playerServer{service->session->server};

        if (type == ID_CONNECTION_ATTEMPT_FAILED) {
            // Unsubscribe from callbacks
            playerServer->player.netPeer.removeCallback(this);
            serverCreationError("Failed to connect player server to NAT server");
            return;
        }

        if (type != ID_CONNECTION_REQUEST_ACCEPTED) {
            return;
        }

        // Unsubscribe from callbacks
        playerServer->player.netPeer.removeCallback(this);

        peer->AttachPlugin(&playerServer->natClient);
        playerServer->natClient.FindRouterPortStride(packet->systemAddress);

        logDebug("lobby.log", "Player server connected to lobby server, nat client attached");
        createHostPlayer();
    }
};

static ServerConnectCallbacks serverConnectCallbacks;

static void createSessionAndServer(const char* sessionName)
{
    logDebug("lobby.log", "Create session");

    auto service = getNetService();
    service->session = (CNetCustomSession*)createCustomNetSession(service, sessionName, true);

    logDebug("lobby.log", "Create player server");

    // NetSession and NetSystem will be set later by the game in CMidServerStart()
    auto playerServer = (CNetCustomPlayerServer*)createCustomPlayerServer(service->session, nullptr,
                                                                          nullptr);

    service->session->server = playerServer;

    const auto& lobbySettings = userSettings().lobby;
    const auto& serverIp = lobbySettings.server.ip;
    const auto& serverPort = lobbySettings.server.port;

    auto peer = playerServer->getPeer();
    // Connect player server to lobby server, wait response.
    // We need this connection for NAT punchthrough
    if (peer->Connect(serverIp.c_str(), serverPort, nullptr, 0)
        != SLNet::CONNECTION_ATTEMPT_STARTED) {
        const std::string msg{"Failed to connect server player to lobby server"};
        logError("lobby.log", msg);
        serverCreationError(msg);
        return;
    }

    logDebug("lobby.log", "Player server waits for response from lobby server");
    playerServer->player.netPeer.addCallback(&serverConnectCallbacks);
}

/** Handles response to room creation request. */
class RoomsCreateServerCallback : public SLNet::RoomsCallback
{
public:
    RoomsCreateServerCallback() = default;
    ~RoomsCreateServerCallback() override = default;

    void CreateRoom_Callback(const SLNet::SystemAddress& senderAddress,
                             SLNet::CreateRoom_Func* callResult) override
    {
        logDebug("lobby.log", "Room creation response received");

        // Unsubscribe from callbacks
        removeRoomsCallback(this);

        if (callResult->resultCode != SLNet::REC_SUCCESS) {
            auto result{SLNet::RoomsErrorCodeDescription::ToEnglish(callResult->resultCode)};
            const auto msg{fmt::format("Could not create a room.\nReason: {:s}", result)};

            logDebug("lobby.log", msg);
            serverCreationError(msg);
            return;
        }

        // Lobby server successfully created a room.
        // Create session object and player server.
        auto cell{callResult->roomDescriptor.GetProperty(DefaultRoomColumns::TC_ROOM_NAME)};
        const char* roomName = cell ? cell->c : "Unknown room";

        logDebug("roomServer.log", "Create session and server");
        createSessionAndServer(roomName);
    }
};

static RoomsCreateServerCallback roomServerCallback;

void startRoomAndServerCreation(game::CMenuBase* menu)
{
    using namespace game;

    menuWaitServer = (CMenuFlashWait*)Memory::get().allocate(sizeof(CMenuFlashWait));
    CMenuFlashWaitApi::get().constructor(menuWaitServer);

    showInterface(menuWaitServer);

    auto& menuApi = CMenuBaseApi::get();
    auto& dialogApi = CDialogInterfApi::get();

    auto dialog = menuApi.getDialogInterface(menu);
    auto editGame = dialogApi.findEditBox(dialog, "EDIT_GAME");

    // Request room creation and wait for lobby server response
    if (!tryCreateRoom(editGame->data->editBoxData.inputString.string)) {
        serverCreationError("Failed to request room creation");
        return;
    }

    logDebug("lobby.log", "Waiting for room creation response");

    menuBase = menu;
    addRoomsCallback(&roomServerCallback);
}

} // namespace hooks
