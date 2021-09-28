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

#ifndef MQNETPLAYERENUM_H
#define MQNETPLAYERENUM_H

namespace game {

struct String;
struct IMqNetPlayerEnumVftable;

struct IMqNetPlayerEnum
{
    IMqNetPlayerEnumVftable* vftable;
};

struct IMqNetPlayerEnumVftable
{
    using Destructor = void(__thiscall*)(IMqNetPlayerEnum* thisptr, char flags);
    Destructor destructor;

    /** Returns player name. */
    using GetName = String*(__thiscall*)(const IMqNetPlayerEnum* thisptr, String* name);
    GetName getName;

    /** Returns player network id. */
    using GetId = int(__thiscall*)(const IMqNetPlayerEnum* thisptr);
    GetId getId;
};

static_assert(sizeof(IMqNetPlayerEnumVftable) == 3 * sizeof(void*),
              "IMqNetPlayerEnum vftable must have exactly 3 methods");

} // namespace game

#endif // MQNETPLAYERENUM_H
