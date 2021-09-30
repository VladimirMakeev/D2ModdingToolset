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

#ifndef MQNETSESSENUM_H
#define MQNETSESSENUM_H

#include "linkedlist.h"

namespace game {

struct String;
struct IMqNetPlayerEnum;
struct IMqNetSessEnumVftable;

/**
 * Interface for thin wrappers that are created during session enumeration.
 * Used to store common session info to show in UI and further connection.
 */
struct IMqNetSessEnum
{
    IMqNetSessEnumVftable* vftable;
};

struct IMqNetSessEnumVftable
{
    using Destructor = void(__thiscall*)(IMqNetSessEnum* thisptr, char flags);
    Destructor destructor;

    /** Returns session name. */
    using GetName = String*(__thiscall*)(const IMqNetSessEnum* thisptr, String* name);
    GetName getName;

    /** Returns current number of players in a session. */
    using GetPlayersCount = int(__thiscall*)(const IMqNetSessEnum* thisptr);
    GetPlayersCount getPlayersCount;

    /** Returns maximum allowed number of players in a session. */
    using GetMaxPlayers = int(__thiscall*)(const IMqNetSessEnum* thisptr);
    GetMaxPlayers getMaxPlayers;

    /** Returns true if session protected with a password. */
    using IsPasswordRequired = bool(__thiscall*)(const IMqNetSessEnum* thisptr);
    IsPasswordRequired isPasswordRequired;

    /** Returns list of players in a session. */
    using GetPlayers = void(__thiscall*)(const IMqNetSessEnum* thisptr,
                                         LinkedList<IMqNetPlayerEnum*>* players);
    GetPlayers getPlayers;
};

static_assert(sizeof(IMqNetSessEnumVftable) == 6 * sizeof(void*),
              "IMqNetSessEnum vftable must have exactly 6 methods");

} // namespace game

#endif // MQNETSESSENUM_H
