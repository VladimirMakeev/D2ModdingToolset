/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef UNITPOSITIONMAP_H
#define UNITPOSITIONMAP_H

#include "d2map.h"
#include "unitpositionpair.h"

namespace game {

using UnitPositionMap = Map<int, bool>;
using UnitPositionMapNode = MapNode<int, bool>;
using UnitPositionMapIterator = MapIterator<int, bool>;

namespace UnitPositionMapApi {

struct Api
{
    using Constructor = UnitPositionMap*(__thiscall*)(UnitPositionMap* thisptr);
    Constructor constructor;

    using Destructor = void(__thiscall*)(UnitPositionMap* thisptr);
    Destructor destructor;

    using CopyConstructor = UnitPositionMap*(__thiscall*)(UnitPositionMap* thisptr,
                                                          const UnitPositionMap* src);
    CopyConstructor copyConstructor;

    using CopyAssignment = UnitPositionMap*(__thiscall*)(UnitPositionMap* thisptr,
                                                         const UnitPositionMap* src);
    CopyAssignment copyAssignment;

    using FindByPosition = UnitPositionPair*(__stdcall*)(UnitPositionPair* value,
                                                         const UnitPositionMap* map,
                                                         int position);
    FindByPosition findByPosition;

    using HasNegativePosition = bool(__stdcall*)(const UnitPositionMap* thisptr);
    HasNegativePosition hasNegativePosition;
};

Api& get();

} // namespace UnitPositionMapApi

} // namespace game

#endif // UNITPOSITIONMAP_H
