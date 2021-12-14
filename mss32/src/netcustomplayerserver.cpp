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

#include "netcustomplayerserver.h"
#include "log.h"
#include "mempool.h"
#include "mqnetreception.h"
#include "mqnetsystem.h"
#include "netcustomplayer.h"
#include "netmsg.h"
#include "utils.h"
#include <BitStream.h>
#include <MessageIdentifiers.h>
#include <array>
#include <fmt/format.h>
#include <mutex>

namespace hooks {

static std::mutex netMessagesMutex;

void __fastcall playerServerPacketCallback(CNetCustomPlayerServer* thisptr, int /*%edx*/)
{
    auto peer{thisptr->player->peer};
    if (!peer) {
        return;
    }

    auto netSystem{thisptr->player->netSystem};

    static std::mutex packetMutex;
    std::lock_guard<std::mutex> guard(packetMutex);

    for (auto packet = peer->Receive(); packet != nullptr;
         peer->DeallocatePacket(packet), packet = peer->Receive()) {

        switch (packet->data[0]) {
        case ID_REMOTE_DISCONNECTION_NOTIFICATION: {
            logDebug("playerServer.log", "Client disconnected");

            if (netSystem) {
                auto guid = peer->GetGuidFromSystemAddress(packet->systemAddress);
                auto guidInt = SLNet::RakNetGUID::ToUint32(guid);

                netSystem->vftable->onPlayerDisconnected(netSystem, (int)guidInt);
            }

            break;
        }
        case ID_REMOTE_CONNECTION_LOST: {
            logDebug("playerServer.log", "Client lost connection");

            if (netSystem) {
                auto guid = peer->GetGuidFromSystemAddress(packet->systemAddress);
                auto guidInt = SLNet::RakNetGUID::ToUint32(guid);

                netSystem->vftable->onPlayerDisconnected(netSystem, (int)guidInt);
            }

            break;
        }
        case ID_REMOTE_NEW_INCOMING_CONNECTION:
            logDebug("playerServer.log", "Client connected");
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED:
            // This should never happen on server ?
            logDebug("playerServer.log", "Connection request to the server was accepted");
            break;
        case ID_NEW_INCOMING_CONNECTION: {
            logDebug("playerServer.log", "Incoming connection");

            if (netSystem) {
                auto guid = peer->GetGuidFromSystemAddress(packet->systemAddress);
                auto guidInt = SLNet::RakNetGUID::ToUint32(guid);

                netSystem->vftable->onPlayerConnected(netSystem, (int)guidInt);
            }

            break;
        }
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            // This should never happen on server ?
            logDebug("playerServer.log", "Server is full");
            break;
        case ID_DISCONNECTION_NOTIFICATION: {
            logDebug("playerServer.log", "Client has disconnected from server");

            if (netSystem) {
                auto guid = peer->GetGuidFromSystemAddress(packet->systemAddress);
                auto guidInt = SLNet::RakNetGUID::ToUint32(guid);

                netSystem->vftable->onPlayerDisconnected(netSystem, (int)guidInt);
            }

            break;
        }
        case ID_CONNECTION_LOST: {
            logDebug("playerServer.log", "Client has lost connection");

            if (netSystem) {
                auto guid = peer->GetGuidFromSystemAddress(packet->systemAddress);
                auto guidInt = SLNet::RakNetGUID::ToUint32(guid);

                netSystem->vftable->onPlayerDisconnected(netSystem, (int)guidInt);
            }

            break;
        }
        case 0xff: {
            // Game message received
            auto message = reinterpret_cast<const game::NetMessageHeader*>(packet->data);

            auto guid = peer->GetGuidFromSystemAddress(packet->systemAddress);
            auto guidInt = SLNet::RakNetGUID::ToUint32(guid);

            /*logDebug("playerServer.log", fmt::format("Game message '{:s}' from {:x}",
                                                     message->messageClassName, guidInt));*/

            // logDebug("playerServer.log", "Allocate net message");

            auto msg = std::make_unique<unsigned char[]>(message->length);
            // logDebug("playerServer.log", "Copy net message");
            std::memcpy(msg.get(), message, message->length);

            {
                std::lock_guard<std::mutex> messageGuard(netMessagesMutex);
                thisptr->messages.push_back(
                    CNetCustomPlayerServer::IdMessagePair{std::uint32_t{guidInt}, std::move(msg)});
            }

            auto reception = thisptr->player->netReception;
            if (reception) {
                reception->vftable->notify(reception);
            }

            break;
        }
        default:
            logDebug("playerServer.log",
                     fmt::format("Packet type {:d}", static_cast<int>(packet->data[0])));
            break;
        }
    }
}

static void __fastcall playerServerDtor(CNetCustomPlayerServer* thisptr, int /*%edx*/, char flags)
{
    playerLog("CNetCustomPlayerServer d-tor");

    playerLog("Remove packet processing event");
    game::UiEventApi::get().destructor(&thisptr->packetEvent);

    thisptr->player->vftable->destructor(thisptr->player, flags);

    if (thisptr->messages.size()) {
        playerLog("Remove net message that was not handled!");
    }

    playerLog("Destroy net messages list");
    thisptr->messages.~list();

    if (flags & 1) {
        playerLog("CNetCustomPlayerServer d-tor frees memory");
        game::Memory::get().freeNonZero(thisptr);
    }
}

static game::String* __fastcall playerServerGetName(CNetCustomPlayerServer* thisptr,
                                                    int /*%edx*/,
                                                    game::String* string)
{
    playerLog("CNetCustomPlayerServer getName");
    thisptr->player->vftable->getName(thisptr->player, string);
    return string;
}

static int __fastcall playerServerGetNetId(CNetCustomPlayerServer* thisptr, int /*%edx*/)
{
    playerLog("CNetCustomPlayerServer getNetId");
    return thisptr->player->vftable->getNetId(thisptr->player);
}

static game::IMqNetSession* __fastcall playerServerGetSession(CNetCustomPlayerServer* thisptr,
                                                              int /*%edx*/)
{
    playerLog("CNetCustomPlayerServer getSession");
    return thisptr->player->vftable->getSession(thisptr->player);
}

static int __fastcall playerServerGetMessageCount(CNetCustomPlayerServer* thisptr, int /*%edx*/)
{
    playerLog("CNetCustomPlayerServer getMessageCount");

    std::lock_guard<std::mutex> messageGuard(netMessagesMutex);

    return static_cast<int>(thisptr->messages.size());
}

static bool __fastcall playerServerSendMessage(CNetCustomPlayerServer* thisptr,
                                               int /*%edx*/,
                                               int idTo,
                                               const game::NetMessageHeader* message)
{
    auto peer{thisptr->player->peer};

    if (!peer) {
        playerLog("CNetCustomPlayerServer could not send message, peer is nullptr");
        return false;
    }

    SLNet::BitStream stream((unsigned char*)message, message->length, false);

    // if idTo == 0, send broadcast message.
    if (idTo == 0) {
        playerLog("CNetCustomPlayerServer sendMessage broadcast");

        peer->Send(&stream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0,
                   SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
        return true;
    }

    // TODO: remember who is connected to us, their netIds and system addresses
    // For now, just find out who is this id owner
    std::array<SLNet::SystemAddress, 4> connections;
    std::uint16_t connected{static_cast<std::uint16_t>(connections.size())};

    peer->GetConnectionList(&connections[0], &connected);
    for (std::uint16_t i = 0; i < connected; ++i) {
        auto guid = peer->GetGuidFromSystemAddress(connections[i]);
        auto guidInt = SLNet::RakNetGUID::ToUint32(guid);

        if (static_cast<std::uint32_t>(idTo) == guidInt) {
            // Found, send
            playerLog(fmt::format("CNetCustomPlayerServer sendMessage to 0x{:x}", guidInt));
            peer->Send(&stream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED,
                       0, connections[i], false);
            return true;
        }
    }

    playerLog(fmt::format("CNetCustomPlayerServer could not send message. No client with id 0x{:x}",
                          uint32_t(idTo)));
    return false;
}

static int __fastcall playerServerReceiveMessage(CNetCustomPlayerServer* thisptr,
                                                 int /*%edx*/,
                                                 int* idFrom,
                                                 game::NetMessageHeader* buffer)
{
    if (!idFrom || !buffer) {
        // This should never happen
        return 3;
    }

    playerLog("CNetCustomPlayerServer receiveMessage");

    std::lock_guard<std::mutex> messageGuard(netMessagesMutex);

    if (thisptr->messages.empty()) {
        return 0;
    }

    const auto& pair = thisptr->messages.front();
    auto message = reinterpret_cast<const game::NetMessageHeader*>(pair.second.get());

    if (message->messageType != game::netMessageNormalType) {
        playerLog("CNetCustomPlayerServer received message with invalid type");
        return 3;
    }

    if (message->length >= game::netMessageMaxLength) {
        playerLog("CNetCustomPlayerServer received message with invalid length");
        return 3;
    }

    playerLog(fmt::format("CNetCustomPlayerServer receiveMessage '{:s}' length {:d} from 0x{:x}",
                          message->messageClassName, message->length, pair.first));

    *idFrom = static_cast<int>(pair.first);
    std::memcpy(buffer, message, message->length);

    thisptr->messages.pop_front();
    return 2;
}

static void __fastcall playerServerSetNetSystem(CNetCustomPlayerServer* thisptr,
                                                int /*%edx*/,
                                                game::IMqNetSystem* netSystem)
{
    playerLog("CNetCustomPlayerServer setNetSystem");
    thisptr->player->vftable->setNetSystem(thisptr->player, netSystem);
}

static int __fastcall playerServerMethod8(CNetCustomPlayerServer* thisptr, int /*%edx*/, int a2)
{
    playerLog("CNetCustomPlayerServer method8");
    return thisptr->player->vftable->method8(thisptr->player, a2);
}

static bool __fastcall playerServerDestroyPlayer(CNetCustomPlayerServer* thisptr,
                                                 int /*%edx*/,
                                                 int playerId)
{
    playerLog("CNetCustomPlayerServer destroyPlayer");
    return false;
}

static bool __fastcall playerServerSetMaxPlayers(CNetCustomPlayerServer* thisptr,
                                                 int /*%edx*/,
                                                 int maxPlayers)
{
    playerLog(fmt::format("CNetCustomPlayerServer setMaxPlayers {:d}", maxPlayers));
    return true;
}

static bool __fastcall playerServerSetAllowJoin(CNetCustomPlayerServer* thisptr,
                                                int /*%edx*/,
                                                bool allowJoin)
{
    playerLog(fmt::format("CNetCustomPlayerServer setAllowJoin {:d}", (int)allowJoin));
    return true;
}

static game::IMqNetPlayerServerVftable playerServerVftable{
    (game::IMqNetPlayerServerVftable::Destructor)playerServerDtor,
    (game::IMqNetPlayerServerVftable::GetName)playerServerGetName,
    (game::IMqNetPlayerServerVftable::GetNetId)playerServerGetNetId,
    (game::IMqNetPlayerServerVftable::GetSession)playerServerGetSession,
    (game::IMqNetPlayerServerVftable::GetMessageCount)playerServerGetMessageCount,
    (game::IMqNetPlayerServerVftable::SendNetMessage)playerServerSendMessage,
    (game::IMqNetPlayerServerVftable::ReceiveMessage)playerServerReceiveMessage,
    (game::IMqNetPlayerServerVftable::SetNetSystem)playerServerSetNetSystem,
    (game::IMqNetPlayerServerVftable::Method8)playerServerMethod8,
    (game::IMqNetPlayerServerVftable::DestroyPlayer)playerServerDestroyPlayer,
    (game::IMqNetPlayerServerVftable::SetMaxPlayers)playerServerSetMaxPlayers,
    (game::IMqNetPlayerServerVftable::SetAllowJoin)playerServerSetAllowJoin,
};

game::IMqNetPlayerServer* createCustomPlayerServer(CNetCustomSession* session,
                                                   game::IMqNetSystem* netSystem,
                                                   game::IMqNetReception* netReception)
{
    using namespace game;

    playerLog("Creating player server peer");
    SLNet::SocketDescriptor descriptor{CNetCustomPlayer::serverPort, nullptr};
    auto peer{SLNet::RakPeerInterface::GetInstance()};

    constexpr std::uint16_t maxConnections{4};
    peer->SetMaximumIncomingConnections(maxConnections);

    const auto result{peer->Startup(maxConnections, &descriptor, 1)};
    if (result != SLNet::StartupResult::RAKNET_STARTED) {
        playerLog("Failed to start peer for CNetCustomPlayerServer");
        SLNet::RakPeerInterface::DestroyInstance(peer);
        return nullptr;
    }

    playerLog("Creating player 'server'");
    // 1 is a server netId hardcoded in game and was also used in DirectPlay.
    auto player{createCustomNetPlayer(session, netSystem, netReception, "SERVER", 1)};

    playerLog("Creating CNetCustomPlayerServer");
    auto server = (CNetCustomPlayerServer*)Memory::get().allocate(sizeof(CNetCustomPlayerServer));
    new (server) CNetCustomPlayerServer(player);

    server->player->peer = peer;
    server->vftable = &playerServerVftable;

    playerLog("Creating player server packet event");
    createTimerEvent(&server->packetEvent, server, playerServerPacketCallback, 100);

    playerLog("Player server created");

    auto netId = SLNet::RakNetGUID::ToUint32(peer->GetMyGUID());
    playerLog(fmt::format("CNetCustomPlayerServer has netId 0x{:x}, ", netId));

    return server;
}

} // namespace hooks
