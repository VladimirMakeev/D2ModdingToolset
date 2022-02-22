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

#ifndef NETCUSTOMPLAYER_H
#define NETCUSTOMPLAYER_H

#include "mqnetplayer.h"
#include "networkpeer.h"
#include <cstdint>
#include <string>

namespace game {
struct IMqNetSystem;
struct IMqNetReception;
} // namespace game

namespace hooks {

struct CNetCustomSession;

struct CNetCustomPlayer : public game::IMqNetPlayer
{
    // Ports for SLNet peer, should be on the same IP as lobby client
    static constexpr std::uint16_t serverPort{60000};
    static constexpr std::uint16_t clientPort{60001};

    CNetCustomPlayer(CNetCustomSession* session,
                     game::IMqNetSystem* netSystem,
                     game::IMqNetReception* netReception,
                     const char* name,
                     NetworkPeer::PeerPtr&& peer,
                     std::uint32_t netId);

    ~CNetCustomPlayer();

    auto getPeer()
    {
        return netPeer.peer.get();
    }

    std::string name;
    NetworkPeer netPeer;
    CNetCustomSession* session;
    game::IMqNetSystem* netSystem;
    game::IMqNetReception* netReception;
    std::uint32_t netId;
};

void playerLog(const std::string& message);

} // namespace hooks

#endif // NETCUSTOMPLAYER_H
