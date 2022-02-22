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

#include "networkpeer.h"
#include "utils.h"
#include <algorithm>

namespace hooks {

void __fastcall packetEventCallback(NetworkPeer* netPeer, int /*%edx*/)
{
    auto peer{netPeer->peer.get()};

    for (auto packet = peer->Receive(); packet != nullptr;
         peer->DeallocatePacket(packet), packet = peer->Receive()) {

        auto type = static_cast<DefaultMessageIDTypes>(packet->data[0]);

        for (auto& callback : netPeer->callbacks) {
            callback->onPacketReceived(type, peer, packet);
        }
    }

    auto& toRemove{netPeer->removeCallbacks};
    if (!toRemove.empty()) {
        auto& callbacks{netPeer->callbacks};

        for (auto& callback : toRemove) {
            callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback),
                            callbacks.end());
        }

        toRemove.clear();
    }
}

NetworkPeer::NetworkPeer(PeerPtr&& peer)
    : peer{std::move(peer)}
{
    createTimerEvent(&packetEvent, this, packetEventCallback, 100);
}

NetworkPeer::~NetworkPeer()
{
    game::UiEventApi::get().destructor(&packetEvent);
}

void NetworkPeer::addCallback(NetworkPeerCallbacks* callback)
{
    if (std::find(callbacks.begin(), callbacks.end(), callback) == callbacks.end()) {
        callbacks.push_back(callback);
    }
}

void NetworkPeer::removeCallback(NetworkPeerCallbacks* callback)
{
    if (std::find(removeCallbacks.begin(), removeCallbacks.end(), callback)
        == removeCallbacks.end()) {
        removeCallbacks.push_back(callback);
    }
}

} // namespace hooks
