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

#ifndef MQNETPLAYER_H
#define MQNETPLAYER_H

namespace game {

struct String;
struct IMqNetPlayerVftable;

template <typename T = IMqNetPlayerVftable>
struct IMqNetPlayerT
{
    const T* vftable;
};

/**
 * Interface for network players.
 * Player can be client or a server.
 * Single player and hotseat game logic also uses this interface.
 */
struct IMqNetPlayer : public IMqNetPlayerT<>
{ };

struct IMqNetPlayerVftable
{
    using Destructor = void(__thiscall*)(IMqNetPlayer* thisptr, char flags);
    Destructor destructor;

    using GetName = String*(__thiscall*)(IMqNetPlayer* thisptr, String* string);
    GetName getName;

    using GetNetId = int(__thiscall*)(IMqNetPlayer* thisptr);
    GetNetId getNetId;

    using Method3 = int(__thiscall*)(IMqNetPlayer* thisptr);
    Method3 method3;

    using Method4 = int(__thiscall*)(IMqNetPlayer* thisptr);
    Method4 method4;

    using Method5 = int(__thiscall*)(IMqNetPlayer* thisptr, int a2, int a3);
    Method5 method5;

    using Method6 = int(__thiscall*)(IMqNetPlayer* thisptr, int a2, int a3);
    Method6 method6;

    using Method7 = int(__thiscall*)(IMqNetPlayer* thisptr, int a2);
    Method7 method7;

    using Method8 = int(__thiscall*)(IMqNetPlayer* thisptr, int a2);
    Method8 method8;
};

static_assert(sizeof(IMqNetPlayerVftable) == 9 * sizeof(void*),
              "IMqNetPlayer vftable must have exactly 9 methods");

} // namespace game

#endif // MQNETPLAYER_H
