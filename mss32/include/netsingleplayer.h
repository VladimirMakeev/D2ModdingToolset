/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#ifndef NETSINGLEPLAYER_H
#define NETSINGLEPLAYER_H

#include "d2string.h"
#include "idlist.h"
#include "mqnetplayer.h"

namespace game {

struct CNetSingleSession;
struct IMqNetReception;

/** Used by both CNetSinglePlayerServer and CNetSinglePlayerClient as internal instance. */
struct CNetSinglePlayer : public IMqNetPlayer
{
    int unknown4;
    IMqNetPlayer* owner; // CNetSinglePlayerServer or CNetSinglePlayerClient
    CNetSingleSession* session;
    int unknown16;
    int unknown20;
    IMqNetReception* reception;
    int netId;
    String playerName;
    IdList unknown48;
    IdList unknown64;
};

assert_size(CNetSinglePlayer, 80);
assert_offset(CNetSinglePlayer, unknown4, 4);
assert_offset(CNetSinglePlayer, unknown16, 16);
assert_offset(CNetSinglePlayer, unknown20, 20);
assert_offset(CNetSinglePlayer, netId, 28);
assert_offset(CNetSinglePlayer, playerName, 32);
assert_offset(CNetSinglePlayer, unknown48, 48);
assert_offset(CNetSinglePlayer, unknown64, 64);

namespace CNetSinglePlayerApi {

IMqNetPlayerVftable* vftable();

} // namespace CNetSinglePlayerApi

} // namespace game

#endif // NETSINGLEPLAYER_H
