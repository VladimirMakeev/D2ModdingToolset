/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef AISTRATPLAYER_H
#define AISTRATPLAYER_H

#include "d2assert.h"

namespace game {

struct IAiStratPlayerVftable;

struct IAiStratPlayer
{
    IAiStratPlayerVftable* vftable;
};

struct IAiStratPlayerVftable
{
    using Destructor = void(__thiscall*)(IAiStratPlayer* thisptr, char flags);
    Destructor destructor;

    void* methods[2];
};

assert_vftable_size(IAiStratPlayerVftable, 3);

} // namespace game

#endif // AISTRATPLAYER_H
