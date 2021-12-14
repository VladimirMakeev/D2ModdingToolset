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

#ifndef NETCUSTOMSERVICE_H
#define NETCUSTOMSERVICE_H

#include "lobbycallbacks.h"
#include "mqnetservice.h"
#include "roomscallback.h"
#include "uievent.h"
#include <Lobby2Client.h>
#include <Lobby2Message.h>
#include <RakPeerInterface.h>
#include <RoomsPlugin.h>
#include <memory>
#include <string>

namespace hooks {

class LoggingCallbacks;
class RoomsLoggingCallback;

struct CNetCustomService : public game::IMqNetService
{
    CNetCustomService(std::unique_ptr<SLNet::Lobby2Client>&& lobbyClient,
                      std::unique_ptr<SLNet::Lobby2MessageFactory>&& msgFactory,
                      std::unique_ptr<LoggingCallbacks>&& logCallbacks,
                      std::unique_ptr<SLNet::RoomsPlugin>&& roomsClient,
                      std::unique_ptr<RoomsLoggingCallback>&& roomsCallback,
                      SLNet::RakPeerInterface* peer);

    std::string loggedAccount;
    SLNet::SystemAddress roomOwnerAddress;

    /** Interacts with lobby server. */
    std::unique_ptr<SLNet::Lobby2Client> lobbyClient;
    /** Creates network messages. */
    std::unique_ptr<SLNet::Lobby2MessageFactory> lobbyMsgFactory;
    /** Callbacks only for debug logging. */
    std::unique_ptr<LoggingCallbacks> loggingCallbacks;
    /** Interacts with lobby server rooms. */
    std::unique_ptr<SLNet::RoomsPlugin> roomsClient;
    std::unique_ptr<RoomsLoggingCallback> roomsLogCallback;
    /** Connection with lobby server. */
    SLNet::RakPeerInterface* lobbyPeer;
    /** Processes network packets from lobby server. */
    game::UiEvent lobbyPacketEvent;
};

CNetCustomService* getNetService();

bool createCustomNetService(game::IMqNetService** service);

void addLobbyCallbacks(SLNet::Lobby2Callbacks* callbacks);
void removeLobbyCallbacks(SLNet::Lobby2Callbacks* callbacks);

void addRoomsCallback(SLNet::RoomsCallback* callback);
void removeRoomsCallback(SLNet::RoomsCallback* callback);

} // namespace hooks

#endif // NETCUSTOMSERVICE_H
