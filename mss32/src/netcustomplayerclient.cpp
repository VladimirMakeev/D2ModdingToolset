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

#include "netcustomplayerclient.h"
#include "log.h"
#include "mempool.h"
#include "mqnetreception.h"
#include "mqnetsystem.h"
#include "netcustomplayer.h"
#include "netcustomservice.h"
#include "netcustomsession.h"
#include "netmessages.h"
#include "netmsg.h"
#include "settings.h"
#include "utils.h"
#include <BitStream.h>
#include <MessageIdentifiers.h>
#include <chrono>
#include <fmt/format.h>
#include <mutex>
#include <thread>

namespace hooks {

static std::mutex netMessagesMutex;

void PlayerClientCallbacks::onPacketReceived(DefaultMessageIDTypes type,
                                             SLNet::RakPeerInterface* peer,
                                             const SLNet::Packet* packet)
{
    auto netSystem{playerClient->player.netSystem};

    switch (type) {
    case ID_REMOTE_DISCONNECTION_NOTIFICATION:
        logDebug("playerClient.log", "Client disconnected");
        break;
    case ID_REMOTE_CONNECTION_LOST:
        logDebug("playerClient.log", "Client lost connection");
        break;
    case ID_REMOTE_NEW_INCOMING_CONNECTION:
        logDebug("playerClient.log", "Client connected");
        break;
    case ID_CONNECTION_REQUEST_ACCEPTED: {
        logDebug("playerClient.log", "Connection request to the server was accepted");

        if (netSystem) {
            auto guid = peer->GetGuidFromSystemAddress(packet->systemAddress);
            auto guidInt = SLNet::RakNetGUID::ToUint32(guid);

            netSystem->vftable->onPlayerConnected(netSystem, (int)guidInt);
        }

        break;
    }
    case ID_NEW_INCOMING_CONNECTION:
        logDebug("playerClient.log", "Incoming connection");
        break;
    case ID_NO_FREE_INCOMING_CONNECTIONS:
        logDebug("playerClient.log", "Server is full");
        break;
    case ID_DISCONNECTION_NOTIFICATION: {
        logDebug("playerClient.log", "Server was shut down");

        if (netSystem) {
            netSystem->vftable->onPlayerDisconnected(netSystem, 1);
        }

        break;
    }
    case ID_CONNECTION_LOST: {
        logDebug("playerClient.log", "Connection with server is lost");

        if (netSystem) {
            netSystem->vftable->onPlayerDisconnected(netSystem, 1);
        }

        break;
    }
    case 0xff: {
        // Game message received
        auto message = reinterpret_cast<const game::NetMessageHeader*>(packet->data);

        auto guid = peer->GetGuidFromSystemAddress(packet->systemAddress);
        auto guidInt = SLNet::RakNetGUID::ToUint32(guid);

        logDebug("playerClient.log",
                 fmt::format("Game message '{:s}' from {:x}", message->messageClassName, guidInt));

        auto msg = std::make_unique<unsigned char[]>(message->length);
        std::memcpy(msg.get(), message, message->length);

        {
            std::lock_guard<std::mutex> messageGuard(netMessagesMutex);
            playerClient->messages.push_back(
                CNetCustomPlayerClient::IdMessagePair{std::uint32_t{guidInt}, std::move(msg)});
        }

        auto reception = playerClient->player.netReception;
        if (reception) {
            reception->vftable->notify(reception);
        }

        break;
    }
    default:
        logDebug("playerClient.log", fmt::format("Packet type {:d}", static_cast<int>(type)));
        break;
    }
}

void __fastcall playerClientDtor(CNetCustomPlayerClient* thisptr, int /*%edx*/, char flags)
{
    playerLog("CNetCustomPlayerClient d-tor");

    thisptr->~CNetCustomPlayerClient();

    if (flags & 1) {
        playerLog("CNetCustomPlayerClient d-tor frees memory");
        game::Memory::get().freeNonZero(thisptr);
    }
}

game::String* __fastcall playerClientGetName(CNetCustomPlayerClient* thisptr,
                                             int /*%edx*/,
                                             game::String* string)
{
    playerLog("CNetCustomPlayerClient getName");
    thisptr->player.vftable->getName(&thisptr->player, string);
    return string;
}

int __fastcall playerClientGetNetId(CNetCustomPlayerClient* thisptr, int /*%edx*/)
{
    playerLog("CNetCustomPlayerClient getNetId");
    return thisptr->player.vftable->getNetId(&thisptr->player);
}

game::IMqNetSession* __fastcall playerClientGetSession(CNetCustomPlayerClient* thisptr,
                                                       int /*%edx*/)
{
    playerLog("CNetCustomPlayerClient getSession");
    return thisptr->player.vftable->getSession(&thisptr->player);
}

int __fastcall playerClientGetMessageCount(CNetCustomPlayerClient* thisptr, int /*%edx*/)
{
    playerLog("CNetCustomPlayerClient getMessageCount");

    std::lock_guard<std::mutex> messageGuard(netMessagesMutex);

    return static_cast<int>(thisptr->messages.size());
}

bool __fastcall playerClientSendMessage(CNetCustomPlayerClient* thisptr,
                                        int /*%edx*/,
                                        int idTo,
                                        const game::NetMessageHeader* message)
{
    auto peer = thisptr->getPeer();
    if (!peer) {
        playerLog("CNetCustomPlayerClient could not send message, peer is nullptr");
        return false;
    }

    auto serverGuid = peer->GetGuidFromSystemAddress(thisptr->serverAddress);
    auto serverId = SLNet::RakNetGUID::ToUint32(serverGuid);

    playerLog(
        fmt::format("CNetCustomPlayerClient {:s} sendMessage '{:s}' to {:x}, server guid 0x{:x}",
                    thisptr->player.name, message->messageClassName, std::uint32_t(idTo),
                    serverId));

    if (idTo != 1) {
        playerLog("CNetCustomPlayerClient should send messages only to server ???");
        // Only send messages to server
        return false;
    }

    SLNet::BitStream stream((unsigned char*)message, message->length, false);

    if (peer->Send(&stream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0,
                   thisptr->serverAddress, false)
        == 0) {
        playerLog(fmt::format("CNetCustomPlayerClient {:s} Send returned bad input",
                              thisptr->player.name));
    }

    return true;
}

int __fastcall playerClientReceiveMessage(CNetCustomPlayerClient* thisptr,
                                          int /*%edx*/,
                                          int* idFrom,
                                          game::NetMessageHeader* buffer)
{
    if (!idFrom || !buffer) {
        // This should never happen
        return 3;
    }

    playerLog("CNetCustomPlayerClient receiveMessage");

    std::lock_guard<std::mutex> messageGuard(netMessagesMutex);

    if (thisptr->messages.empty()) {
        return 0;
    }

    const auto& pair = thisptr->messages.front();
    const auto& id{pair.first};
    if (id != thisptr->serverId) {
        playerLog(
            fmt::format("CNetCustomPlayerClient received message from {:x}, its not a server!",
                        id));
        return 0;
    }

    auto message = reinterpret_cast<const game::NetMessageHeader*>(pair.second.get());

    if (message->messageType != game::netMessageNormalType) {
        playerLog("CNetCustomPlayerClient received message with invalid type");
        return 3;
    }

    if (message->length >= game::netMessageMaxLength) {
        playerLog("CNetCustomPlayerClient received message with invalid length");
        return 3;
    }

    playerLog(fmt::format("CNetCustomPlayerClient receiveMessage '{:s}' length {:d} from 0x{:x}",
                          message->messageClassName, message->length, pair.first));

    *idFrom = static_cast<int>(id);
    std::memcpy(buffer, message, message->length);

    thisptr->messages.pop_front();
    return 2;
}

void __fastcall playerClientSetNetSystem(CNetCustomPlayerClient* thisptr,
                                         int /*%edx*/,
                                         game::IMqNetSystem* netSystem)
{
    playerLog("CNetCustomPlayerClient setNetSystem");
    thisptr->player.vftable->setNetSystem(&thisptr->player, netSystem);
}

int __fastcall playerClientMethod8(CNetCustomPlayerClient* thisptr, int /*%edx*/, int a2)
{
    playerLog("CNetCustomPlayerClient method8");
    return thisptr->player.vftable->method8(&thisptr->player, a2);
}

static bool __fastcall playerClientSetName(CNetCustomPlayerClient* thisptr,
                                           int /*%edx*/,
                                           const char* name)
{
    playerLog("CNetCustomPlayerClient setName");
    thisptr->player.name = name;
    return true;
}

static bool __fastcall playerClientIsHost(CNetCustomPlayerClient* thisptr, int /*%edx*/)
{
    const auto host{thisptr->player.session->host};
    playerLog(fmt::format("CNetCustomPlayerClient isHost {:d}", host));
    return host;
}

static game::IMqNetPlayerClientVftable playerClientVftable{
    (game::IMqNetPlayerClientVftable::Destructor)playerClientDtor,
    (game::IMqNetPlayerClientVftable::GetName)playerClientGetName,
    (game::IMqNetPlayerClientVftable::GetNetId)playerClientGetNetId,
    (game::IMqNetPlayerClientVftable::GetSession)playerClientGetSession,
    (game::IMqNetPlayerClientVftable::GetMessageCount)playerClientGetMessageCount,
    (game::IMqNetPlayerClientVftable::SendNetMessage)playerClientSendMessage,
    (game::IMqNetPlayerClientVftable::ReceiveMessage)playerClientReceiveMessage,
    (game::IMqNetPlayerClientVftable::SetNetSystem)playerClientSetNetSystem,
    (game::IMqNetPlayerClientVftable::Method8)playerClientMethod8,
    (game::IMqNetPlayerClientVftable::SetName)playerClientSetName,
    (game::IMqNetPlayerClientVftable::IsHost)playerClientIsHost,
};

CNetCustomPlayerClient::CNetCustomPlayerClient(CNetCustomSession* session,
                                               game::IMqNetSystem* netSystem,
                                               game::IMqNetReception* netReception,
                                               const char* name,
                                               NetworkPeer::PeerPtr&& peer,
                                               std::uint32_t netId,
                                               const SLNet::SystemAddress& serverAddress,
                                               std::uint32_t serverId)
    : player{session, netSystem, netReception, name, std::move(peer), netId}
    , callbacks(this)
    , serverAddress{serverAddress}
    , serverId{serverId}
{
    vftable = &playerClientVftable;

    playerLog("Setup player client packet callbacks");
    player.netPeer.addCallback(&callbacks);
}

SLNet::SystemAddress lobbyAddressToServerPlayer(const SLNet::SystemAddress& lobbyAddress);

game::IMqNetPlayerClient* createCustomPlayerClient(CNetCustomSession* session,
                                                   game::IMqNetSystem* netSystem,
                                                   game::IMqNetReception* netReception,
                                                   const char* name)
{
    using namespace game;

    const std::uint16_t clientPort = CNetCustomPlayer::clientPort
                                     + userSettings().lobby.client.port;
    SLNet::SocketDescriptor descriptor{clientPort, nullptr};
    auto peer{NetworkPeer::PeerPtr(SLNet::RakPeerInterface::GetInstance())};

    const auto result{peer->Startup(1, &descriptor, 1)};
    if (result != SLNet::StartupResult::RAKNET_STARTED) {
        playerLog("Failed to start peer for CNetCustomPlayerClient");
        return nullptr;
    }

    auto service = session->service;
    auto serverAddress{lobbyAddressToServerPlayer(service->lobbyPeer.peer->GetMyBoundAddress())};

    auto serverIp{serverAddress.ToString(false)};
    playerLog(fmt::format("Client tries to connect to server at '{:s}'", serverIp));

    // connect to server
    const auto connectResult{peer->Connect(serverIp, CNetCustomPlayer::serverPort, nullptr, 0)};
    if (connectResult != SLNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED) {
        playerLog(fmt::format("Failed to start CNetCustomPlayerClient connection. Error: {:d}",
                              (int)connectResult));
        return nullptr;
    }

    {
        playerLog("Wait for fucks sake");
        using namespace std::chrono_literals;
        // This delay is needed for SLNet peer to connect to server
        // so the peer->GetGuidFromSystemAddress() call will work and give us server netId.
        // This is ugly and unreliable at all.
        // We can just store server SystemAddress and convert it go guid
        // or compare with packet->systemAddress to filter messages from server.

        // Or we just assume that players in game never send messages between each other
        // only to and from the server?
        std::this_thread::sleep_for(100ms);
    }

    auto netId = SLNet::RakNetGUID::ToUint32(peer->GetMyGUID());

    playerLog(fmt::format("Creating player client on port {:d}", clientPort));

    auto serverGuid = peer->GetGuidFromSystemAddress(
        SLNet::SystemAddress(serverIp, CNetCustomPlayer::serverPort));
    auto serverId = SLNet::RakNetGUID::ToUint32(serverGuid);

    playerLog("Creating CNetCustomPlayerClient");
    auto client = (CNetCustomPlayerClient*)Memory::get().allocate(sizeof(CNetCustomPlayerClient));
    new (client) CNetCustomPlayerClient(session, netSystem, netReception, name, std::move(peer),
                                        netId, serverAddress, serverId);

    playerLog("Player client created");

    playerLog(fmt::format("CNetCustomPlayerClient has netId 0x{:x}, "
                          "expects server with netId 0x{:x}",
                          netId, serverId));

    return client;
}

} // namespace hooks
