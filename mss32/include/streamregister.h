/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef STREAMREGISTERBASE_H
#define STREAMREGISTERBASE_H

#include "d2assert.h"

namespace game {

struct CStreamRegisterBaseVftable;
struct IMidScenarioObject;
struct CMidgardID;

struct CStreamRegisterBase
{
    CStreamRegisterBaseVftable* vftable;
};

assert_size(CStreamRegisterBase, 4);

struct CStreamRegisterBaseVftable
{
    using Destructor = void(__thiscall*)(CStreamRegisterBase* thisptr, char flags);
    Destructor destructor;

    /** Creates scenario object with specified id. Object type depends on child class template argument. */
    using Stream = IMidScenarioObject*(__thiscall*)(CStreamRegisterBase* thisptr, const CMidgardID* objectId);
    Stream stream;
};

template<typename T>
struct CStreamRegister : public CStreamRegisterBase
{ };

}


#endif // STREAMREGISTERBASE_H