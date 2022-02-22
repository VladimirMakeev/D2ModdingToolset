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

#ifndef NETWORKPEER_H
#define NETWORKPEER_H

#include "uievent.h"
#include <MessageIdentifiers.h>
#include <RakPeerInterface.h>
#include <memory>
#include <vector>

namespace hooks {

class NetworkPeerCallbacks
{
public:
    virtual ~NetworkPeerCallbacks() = default;

    virtual void onPacketReceived(DefaultMessageIDTypes type,
                                  SLNet::RakPeerInterface* peer,
                                  const SLNet::Packet* packet) = 0;
};

/**
 * Wraps SLikeNet peer interface and game timer event for network packets processing.
 * Allows users to subscribe to peer packet events.
 */
struct NetworkPeer
{
    struct PeerDeleter
    {
        void operator()(SLNet::RakPeerInterface* peer)
        {
            peer->Shutdown(100);
            SLNet::RakPeerInterface::DestroyInstance(peer);
        }
    };

    using PeerPtr = std::unique_ptr<SLNet::RakPeerInterface, PeerDeleter>;

    NetworkPeer(PeerPtr&& peer);
    ~NetworkPeer();

    void addCallback(NetworkPeerCallbacks* callback);
    void removeCallback(NetworkPeerCallbacks* callback);

    game::UiEvent packetEvent{};
    std::vector<NetworkPeerCallbacks*> callbacks;
    std::vector<NetworkPeerCallbacks*> removeCallbacks;
    PeerPtr peer;
};

} // namespace hooks

#endif // NETWORKPEER_H
