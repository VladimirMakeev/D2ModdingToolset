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

#ifndef MQNETPLAYERSERVER_H
#define MQNETPLAYERSERVER_H

#include "mqnetplayer.h"

namespace game {

struct IMqNetPlayerServerVftable;

/** Interface for network server. */
struct IMqNetPlayerServer : public IMqNetPlayerT<IMqNetPlayerServerVftable>
{ };

struct IMqNetPlayerServerVftable : public IMqNetPlayerVftable
{
    /**
     * Destroys remote player with specified id.
     * CNetDPlayPlayerServer calls IDirectPlay4::DestroyPlayer here.
     */
    using DestroyPlayer = bool(__thiscall*)(IMqNetPlayerServer* thisptr, int playerId);
    DestroyPlayer destroyPlayer;

    /** Set maximum client players allowed in session. */
    using SetMaxPlayers = bool(__thiscall*)(IMqNetPlayerServer* thisptr, int maxPlayers);
    SetMaxPlayers setMaxPlayers;

    /** Allows or disables new client players joining session. */
    using SetAllowJoin = bool(__thiscall*)(IMqNetPlayerServer* thisptr, bool allowJoin);
    SetAllowJoin setAllowJoin;
};

assert_vftable_size(IMqNetPlayerServerVftable, 12);

} // namespace game

#endif // MQNETPLAYERSERVER_H
