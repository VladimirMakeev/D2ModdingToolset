/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef MQNETSESSION_H
#define MQNETSESSION_H

#include "linkedlist.h"

namespace game {

struct String;
struct IMqNetPlayerEnum;
struct IMqNetPlayerClient;
struct IMqNetPlayerServer;
struct IMqNetReception;
struct IMqNetSystem;
struct IMqNetSessionVftable;

struct IMqNetSession
{
    IMqNetSessionVftable* vftable;
};

struct IMqNetSessionVftable
{
    using Destructor = void(__thiscall*)(IMqNetSession* thisptr, char flags);
    Destructor destructor;

    /** Returns session name. */
    using GetName = String*(__thiscall*)(IMqNetSession* thisptr, String* sessionName);
    GetName getName;

    /** Returns current number of clients in a session. */
    using GetClientsCount = int(__thiscall*)(IMqNetSession* thisptr);
    GetClientsCount getClientsCount;

    /** Returns the maximum allowed number of clients in a session. */
    using GetMaxClients = int(__thiscall*)(IMqNetSession* thisptr);
    GetMaxClients getMaxClients;

    using GetPlayers = void(__thiscall*)(IMqNetSession* thisptr,
                                         LinkedList<IMqNetPlayerEnum*>* players);
    GetPlayers getPlayers;

    /**
     * Creates client player.
     * CNetDPlaySession calls IDirectPlay4::CreatePlayer with dwFlags = 0 here.
     */
    using CreateClient = void(__thiscall*)(IMqNetSession* thisptr,
                                           IMqNetPlayerClient** client,
                                           IMqNetSystem* netSystem,
                                           IMqNetReception* reception,
                                           const char* clientName);
    CreateClient createClient;

    /**
     * Creates server player.
     * CNetDPlaySession calls IDirectPlay4::CreatePlayer with DPPLAYER_SERVERPLAYER here.
     */
    using CreateServer = void(__thiscall*)(IMqNetSession* thisptr,
                                           IMqNetPlayerServer** server,
                                           IMqNetSystem* netSystem,
                                           IMqNetReception* reception);
    CreateServer createServer;
};

static_assert(sizeof(IMqNetSessionVftable) == 7 * sizeof(void*),
              "IMqNetSession vftable must have exactly 7 methods");

} // namespace game

#endif // MQNETSESSION_H
