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

#ifndef MQNETPLAYERCLIENT_H
#define MQNETPLAYERCLIENT_H

#include "mqnetplayer.h"

namespace game {

struct IMqNetPlayerClientVftable;

/** Interface for network clients. */
struct IMqNetPlayerClient : public IMqNetPlayerT<IMqNetPlayerClientVftable>
{ };

struct IMqNetPlayerClientVftable : public IMqNetPlayerVftable
{
    using Method9 = bool(__thiscall*)(IMqNetPlayerClient* thisptr, int a2);
    Method9 method9;

    using Method10 = bool(__thiscall*)(IMqNetPlayerClient* thisptr, const char* string);
    Method10 method10;
};

static_assert(sizeof(IMqNetPlayerClientVftable) == 11 * sizeof(void*),
              "IMqNetPlayerClient vftable must have exactly 11 methods");

} // namespace game

#endif // MQNETPLAYERCLIENT_H
