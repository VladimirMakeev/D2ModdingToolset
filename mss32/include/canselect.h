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

#ifndef CANSELECT_H
#define CANSELECT_H

#include "d2assert.h"

namespace game {

struct CMqPoint;
struct ICanSelectVftable;

struct ICanSelect
{
    ICanSelectVftable* vftable;
};

struct ICanSelectVftable
{
    using Destructor = void(__thiscall*)(ICanSelect* thisptr, char flags);
    Destructor destructor;

    using CanSelect = bool(__thiscall*)(ICanSelect* thisptr, const CMqPoint* mapPosition);
    CanSelect canSelect;
};

assert_vftable_size(ICanSelectVftable, 2);

} // namespace game

#endif // CANSELECT_H
