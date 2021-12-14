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

#ifndef NETCUSTOMPLAYERCLIENT_H
#define NETCUSTOMPLAYERCLIENT_H

#include "mqnetplayerclient.h"
#include "uievent.h"
#include <list>
#include <memory>
#include <slikenet/types.h>
#include <utility>

namespace game {
struct IMqNetReception;
}

namespace hooks {

struct CNetCustomPlayer;
struct CNetCustomSession;

struct CNetCustomPlayerClient : public game::IMqNetPlayerClient
{
    CNetCustomPlayerClient(CNetCustomPlayer* player)
        : player{player}
    { }

    CNetCustomPlayer* player;
    /** Processes network packets from player server. */
    game::UiEvent packetEvent;
    SLNet::SystemAddress serverAddress;
    std::uint32_t serverId;

    using NetMessagePtr = std::unique_ptr<unsigned char[]>;
    using IdMessagePair = std::pair<std::uint32_t, NetMessagePtr>;

    std::list<IdMessagePair> messages;
};

game::IMqNetPlayerClient* createCustomPlayerClient(CNetCustomSession* session,
                                                   game::IMqNetSystem* netSystem,
                                                   game::IMqNetReception* netReception,
                                                   const char* name);

} // namespace hooks

#endif // NETCUSTOMPLAYERCLIENT_H
