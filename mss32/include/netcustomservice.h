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
#include "networkpeer.h"
#include "roomscallback.h"
#include <Lobby2Client.h>
#include <Lobby2Message.h>
#include <RakPeerInterface.h>
#include <RoomsPlugin.h>
#include <memory>
#include <string>

namespace hooks {

struct CNetCustomService;

class LobbyPeerCallbacks : public NetworkPeerCallbacks
{
public:
    LobbyPeerCallbacks(CNetCustomService* netService)
        : netService{netService}
    { }

    virtual ~LobbyPeerCallbacks() = default;

    void onPacketReceived(DefaultMessageIDTypes type,
                          SLNet::RakPeerInterface* peer,
                          const SLNet::Packet* packet) override;

private:
    CNetCustomService* netService;
};

struct CNetCustomService : public game::IMqNetService
{
    CNetCustomService(NetworkPeer::PeerPtr&& peer);
    ~CNetCustomService() = default;

    std::string loggedAccount;
    SLNet::SystemAddress roomOwnerAddress;

    /** Interacts with lobby server. */
    SLNet::Lobby2Client lobbyClient;
    /** Creates network messages. */
    SLNet::Lobby2MessageFactory lobbyMsgFactory;
    /** Callbacks only for debug logging. */
    LoggingCallbacks loggingCallbacks;
    /** Interacts with lobby server rooms. */
    SLNet::RoomsPlugin roomsClient;
    RoomsLoggingCallback roomsLogCallback;
    /** Connection with lobby server. */
    NetworkPeer lobbyPeer;
    LobbyPeerCallbacks callbacks;
};

CNetCustomService* getNetService();

bool createCustomNetService(game::IMqNetService** service);

void addLobbyCallbacks(SLNet::Lobby2Callbacks* callbacks);
void removeLobbyCallbacks(SLNet::Lobby2Callbacks* callbacks);

void addRoomsCallback(SLNet::RoomsCallback* callback);
void removeRoomsCallback(SLNet::RoomsCallback* callback);

} // namespace hooks

#endif // NETCUSTOMSERVICE_H
