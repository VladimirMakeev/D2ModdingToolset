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
struct IMqNetSessionVftable;

struct IMqNetSession
{
    IMqNetSessionVftable* vftable;
};

struct IMqNetSessionVftable
{
    using Destructor = void(__thiscall*)(IMqNetSession* thisptr, char flags);
    Destructor destructor;

    using Method1 = String*(__thiscall*)(IMqNetSession* thisptr, String* string);
    Method1 method1;

    using Method2 = int(__thiscall*)(IMqNetSession* thisptr);
    Method2 method2;

    using Method3 = bool(__thiscall*)(IMqNetSession* thisptr);
    Method3 method3;

    using GetPlayers = void(__thiscall*)(IMqNetSession* thisptr,
                                         LinkedList<IMqNetPlayerEnum*>* players);
    GetPlayers getPlayers;

    using CreateClient = void(__thiscall*)(IMqNetSession* thisptr,
                                           IMqNetPlayerClient** client,
                                           int a3,
                                           IMqNetReception* reception,
                                           const char* clientName);
    CreateClient createClient;

    using CreateServer = void(__thiscall*)(IMqNetSession* thisptr,
                                           IMqNetPlayerServer** server,
                                           int a3,
                                           int a4);
    CreateServer createServer;
};

static_assert(sizeof(IMqNetSessionVftable) == 7 * sizeof(void*),
              "IMqNetSession vftable must have exactly 7 methods");

} // namespace game

#endif // MQNETSESSION_H
