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

#include "roomclientjoin.h"
#include "log.h"
#include "menucustomlobby.h"
#include "menuphase.h"
#include "midgard.h"
#include "netcustomplayerclient.h"
#include "netcustomservice.h"
#include "netcustomsession.h"
#include "netmessages.h"
#include "settings.h"
#include <fmt/format.h>

namespace hooks {

static CMenuCustomLobby* menuLobby{nullptr};
static SLNet::RakNetGUID playerServerGuid{};

static void registerClientPlayerAndJoin()
{
    using namespace game;

    auto& midgardApi = CMidgardApi::get();
    auto midgard = midgardApi.instance();
    auto netService = getNetService();

    // Create player using session
    logDebug("roomJoin.log", "Try create net client using midgard");
    auto playerId = midgardApi.createNetClient(midgard, netService->loggedAccount.c_str(), false);

    logDebug("roomJoin.log", fmt::format("Check net client id 0x{:x}", playerId));
    if (playerId == 0) {
        // Network id of zero is invalid
        logDebug("roomJoin.log", "Net client has zero id");
        customLobbyProcessJoinError(menuLobby, "Created net client has net id 0");
        return;
    }

    logDebug("roomJoin.log", "Set menu phase as a net client proxy");

    auto menuPhase = menuLobby->menuBaseData->menuPhase;
    // Set menu phase as net proxy
    midgardApi.setClientsNetProxy(midgard, menuPhase);

    // Get max players from session
    auto netSession{netService->session};
    auto maxPlayers = netSession->vftable->getMaxClients(netSession);

    logDebug("roomJoin.log", fmt::format("Get max number of players in session: {:d}", maxPlayers));

    menuPhase->data->maxPlayers = maxPlayers;

    CMenusReqVersionMsg requestVersion;
    requestVersion.vftable = NetMessagesApi::getMenusReqVersionVftable();

    logDebug("roomJoin.log", "Send version request message to server");
    // Send CMenusReqVersionMsg to server
    // If failed, hide wait message and show error, enable join
    if (!midgardApi.sendNetMsgToServer(midgard, &requestVersion)) {
        customLobbyProcessJoinError(menuLobby, "Could not request game version from server");
        return;
    }

    // Response will be handled by CMenuCustomLobby menuGameVersionMsgHandler
    logDebug("roomJoin.log", "Request sent, wait for callback");
}

/**
 * Handles player client connection to player server.
 * Starts joining the game by player client using game-specific protocol.
 */
class ClientToServerConnectCallbacks : public NetworkPeerCallbacks
{
public:
    ClientToServerConnectCallbacks() = default;
    ~ClientToServerConnectCallbacks() override = default;

    void onPacketReceived(DefaultMessageIDTypes type,
                          SLNet::RakPeerInterface* peer,
                          const SLNet::Packet* packet) override
    {
        auto service = getNetService();
        auto playerClient{service->session->players[0]};

        if (type == ID_CONNECTION_ATTEMPT_FAILED) {
            // Unsubscribe from callbacks
            playerClient->player.netPeer.removeCallback(this);
            customLobbyProcessJoinError(menuLobby,
                                        "Failed to connect player client to player server");
            return;
        }

        if (type != ID_CONNECTION_REQUEST_ACCEPTED) {
            return;
        }

        auto& player{playerClient->player};
        // Unsubscribe from callbacks
        player.netPeer.removeCallback(this);

        logDebug("roomJoin.log", "Player client finally connected to player server. "
                                 "Continue with game protocol");

        // Fully connected!
        // Remember server playerNetId, and our netId
        player.netId = SLNet::RakNetGUID::ToUint32(peer->GetMyGUID());

        auto serverGuid = peer->GetGuidFromSystemAddress(packet->systemAddress);
        playerClient->serverAddress = packet->systemAddress;
        playerClient->serverId = SLNet::RakNetGUID::ToUint32(serverGuid);

        playerClient->setupPacketCallbacks();

        registerClientPlayerAndJoin();
    }
};

static ClientToServerConnectCallbacks clientToServerConnectCallback;

/**
 * Handles NAT punchthrough responses.
 * Connects player client to player server on successfull NAT punch.
 */
class NATPunchCallbacks : public NetworkPeerCallbacks
{
public:
    NATPunchCallbacks() = default;
    ~NATPunchCallbacks() override = default;

    void onPacketReceived(DefaultMessageIDTypes type,
                          SLNet::RakPeerInterface* peer,
                          const SLNet::Packet* packet) override
    {
        const char* error{nullptr};

        switch (type) {
        default:
            return;
        case ID_NAT_TARGET_NOT_CONNECTED:
            error = "Player server is not connected\n"
                    "to the lobby server";
            break;
        case ID_NAT_TARGET_UNRESPONSIVE:
            error = "Player server is not responding";
            break;
        case ID_NAT_CONNECTION_TO_TARGET_LOST:
            error = "Lobby server lost the connection\n"
                    "to the player server\n"
                    "while setting up punchthrough";
            break;
        case ID_NAT_PUNCHTHROUGH_FAILED:
            error = "NAT punchthrough failed";
            break;

        case ID_NAT_PUNCHTHROUGH_SUCCEEDED: {
            auto service = getNetService();
            auto playerClient{service->session->players[0]};
            auto& player{playerClient->player};

            // Unsubscribe from callbacks
            player.netPeer.removeCallback(this);

            logDebug("roomJoin.log",
                     fmt::format("NAT punch succeeded! Player server address {:s}, netId 0x{:x}",
                                 playerClient->serverAddress.ToString(), playerClient->serverId));

            auto result{peer->Connect(packet->systemAddress.ToString(false),
                                      packet->systemAddress.GetPort(), nullptr, 0)};
            if (result != SLNet::CONNECTION_ATTEMPT_STARTED) {
                const auto msg{fmt::format(
                    "Failed to connect client player to server player after NAT punch.\nResult {:d}",
                    (int)result)};
                logError("roomJoin.log", msg);
                customLobbyProcessJoinError(menuLobby, msg.c_str());
                return;
            }

            logDebug("roomJoin.log", "Connection attempt after NAT punch, wait response");
            player.netPeer.addCallback(&clientToServerConnectCallback);
            return;
        }
        }

        if (error) {
            auto service = getNetService();
            auto playerClient{service->session->players[0]};

            // Unsubscribe from callbacks
            playerClient->player.netPeer.removeCallback(this);
            customLobbyProcessJoinError(menuLobby, error);
        }
    }
};

static NATPunchCallbacks natPunchCallbacks;

/**
 * Handles player client connection to lobby server.
 * Starts NAT punchthrough on successfull connection.
 */
class ClientConnectCallbacks : public NetworkPeerCallbacks
{
public:
    ClientConnectCallbacks() = default;
    ~ClientConnectCallbacks() override = default;

    void onPacketReceived(DefaultMessageIDTypes type,
                          SLNet::RakPeerInterface* peer,
                          const SLNet::Packet* packet) override
    {
        auto service = getNetService();
        auto playerClient{service->session->players[0]};

        if (type == ID_CONNECTION_ATTEMPT_FAILED) {
            // Unsubscribe from callbacks
            playerClient->player.netPeer.removeCallback(this);
            customLobbyProcessJoinError(menuLobby, "Failed to connect player client to NAT server");
            return;
        }

        if (type != ID_CONNECTION_REQUEST_ACCEPTED) {
            return;
        }

        logDebug("roomJoin.log", fmt::format("OpenNAT to player server with id 0x{:x}",
                                             SLNet::RakNetGUID::ToUint32(playerServerGuid)));
        // Unsubscribe from callbacks
        playerClient->player.netPeer.removeCallback(this);

        // Start NAT punchthrough
        peer->AttachPlugin(&playerClient->natClient);
        playerClient->natClient.OpenNAT(playerServerGuid, packet->systemAddress);

        // Attach callback, wait response
        playerClient->player.netPeer.addCallback(&natPunchCallbacks);
    }
};

static ClientConnectCallbacks clientConnectCallbacks;

void customLobbyProcessJoin(CMenuCustomLobby* menu,
                            const char* roomName,
                            const SLNet::RakNetGUID& serverGuid)
{
    logDebug("roomJoin.log", "Unsubscribe from rooms list updates while joining the room");

    // Unsubscribe from rooms list notification while we joining the room
    // Remove timer event
    game::UiEventApi::get().destructor(&menu->roomsListEvent);

    // Disconnect ui-related rooms callbacks
    removeRoomsCallback(menu->roomsCallbacks.get());
    menu->roomsCallbacks.reset(nullptr);

    using namespace game;

    menuLobby = menu;
    playerServerGuid = serverGuid;

    // Create session ourselves
    logDebug("roomJoin.log", fmt::format("Process join to {:s}", roomName));

    auto netService = getNetService();
    logDebug("roomJoin.log", fmt::format("Get netService {:p}", (void*)netService));

    if (!netService) {
        customLobbyProcessJoinError(menu, "Net service is null");
        return;
    }

    auto netSession = (CNetCustomSession*)createCustomNetSession(netService, roomName, false);

    logDebug("roomJoin.log", fmt::format("Created netSession {:p}", (void*)netSession));

    // If failed, hide wait message and show error, enable join
    if (!netSession) {
        customLobbyProcessJoinError(menu, "Could not create net session");
        return;
    }

    // Set net session to midgard
    auto& midgardApi = CMidgardApi::get();

    auto midgard = midgardApi.instance();
    auto currentSession{midgard->data->netSession};
    if (currentSession) {
        currentSession->vftable->destructor(currentSession, 1);
    }

    logDebug("roomJoin.log", "Set netSession to midgard");
    midgard->data->netSession = netSession;
    netService->session = netSession;

    logDebug("roomJoin.log", "Mark self as client");
    // Mark self as client
    midgard->data->host = false;

    logDebug("roomJoin.log", "Create player client beforehand");

    // Create player client beforehand
    auto clientPlayer = createCustomPlayerClient(netSession, "");
    netSession->players.push_back(clientPlayer);

    logDebug("roomJoin.log", "Connect to lobby server, wait response");

    // Connect to lobby server, wait response
    const auto& lobbySettings = userSettings().lobby;
    const auto& serverIp = lobbySettings.server.ip;
    const auto& serverPort = lobbySettings.server.port;

    auto peer = clientPlayer->getPeer();
    // Connect player server to lobby server, wait response.
    // We need this connection for NAT punchthrough
    if (peer->Connect(serverIp.c_str(), serverPort, nullptr, 0)
        != SLNet::CONNECTION_ATTEMPT_STARTED) {
        const std::string msg{"Failed to connect client player to lobby server"};
        logError("roomJoin.log", msg);
        customLobbyProcessJoinError(menu, msg.c_str());
        return;
    }

    clientPlayer->player.netPeer.addCallback(&clientConnectCallbacks);
}

} // namespace hooks
