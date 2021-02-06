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

#ifndef INTERFACE_H
#define INTERFACE_H

#include "interfacechilds.h"
#include "smartptr.h"
#include <cstddef>

namespace game {

struct CInterfaceVftable;

struct CInterfaceData
{
    SmartPointer interfManagerImpl;
    int unknown;
    int unknown2;
    int unknown3;
    int unknown4;
    int unknown5;
    CInterfaceChilds childs;
    SmartPointer unknown6;
    SmartPointer unknown7;
};

static_assert(sizeof(CInterfaceData) == 64,
              "Size of CInterfaceData structure must be exactly 64 bytes");

static_assert(offsetof(CInterfaceData, childs) == 28,
              "CInterfaceData::childs offset must be 28 bytes");

/**
 * Base class for all ui elements.
 */
struct CInterface
{
    CInterfaceVftable* vftable;
    CInterfaceData* interfaceData;
};

struct CInterfaceVftable
{
    using Destructor = void(__thiscall*)(CInterface* thisptr, char flags);
    Destructor destructor;

    void* methods[33];
};

static_assert(sizeof(CInterfaceVftable) == 34 * sizeof(void*),
              "CInterface vftable must have exactly 34 methods");

} // namespace game

#endif // INTERFACE_H
