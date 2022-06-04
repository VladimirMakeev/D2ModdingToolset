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

#ifndef MAPELEMENT_H
#define MAPELEMENT_H

#include "d2assert.h"
#include "mqpoint.h"

namespace game {

struct IMapElementVftable;
struct IMidgardObjectMap;
struct CMidgardID;

struct IMapElement
{
    const IMapElementVftable* vftable;
    CMqPoint position;
    int sizeX;
    int sizeY;
};

assert_size(IMapElement, 20);

struct IMapElementVftable
{
    using Destructor = void(__thiscall*)(IMapElement* thisptr, char flags);
    Destructor destructor;

    using Initialize = bool(__thiscall*)(IMapElement* thisptr,
                                         const IMidgardObjectMap* objectMap,
                                         const CMidgardID* leaderId,
                                         const CMidgardID* ownerId,
                                         const CMidgardID* subraceId,
                                         const CMqPoint* position);
    Initialize initialize;

    void* method2;
};

assert_vftable_size(IMapElementVftable, 3);

} // namespace game

#endif // MAPELEMENT_H
