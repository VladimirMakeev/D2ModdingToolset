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

#ifndef UNITPOSITIONSET_H
#define UNITPOSITIONSET_H

#include "d2pair.h"
#include "d2set.h"
#include "unitpositionpair.h"

namespace game {

using UnitPositionSet = Set<UnitPositionPair>;
using UnitPositionSetNode = SetNode<UnitPositionPair>;
using UnitPositionSetIterator = SetIterator<UnitPositionPair>;

namespace UnitPositionSetApi {

struct Api
{
    using Constructor = UnitPositionSet*(__thiscall*)(UnitPositionSet* thisptr);
    Constructor constructor;

    using Destructor = void(__thiscall*)(UnitPositionSet* thisptr);
    Destructor destructor;

    using CopyConstructor = UnitPositionSet*(__thiscall*)(UnitPositionSet* thisptr,
                                                          const UnitPositionSet* src);
    CopyConstructor copyConstructor;

    using CopyAssignment = UnitPositionSet*(__thiscall*)(UnitPositionSet* thisptr,
                                                         const UnitPositionSet* src);
    CopyAssignment copyAssignment;

    using GetIterator = UnitPositionSetIterator*(__thiscall*)(UnitPositionSet* thisptr,
                                                              UnitPositionSetIterator* iterator);
    GetIterator begin;
    GetIterator end;

    using Dereference = UnitPositionPair*(__thiscall*)(UnitPositionSetIterator* thisptr);
    Dereference dereference;

    using Equals = bool(__thiscall*)(UnitPositionSetIterator* thisptr,
                                     const UnitPositionSetIterator* value);
    Equals equals;

    using Preincrement = UnitPositionSetIterator*(__thiscall*)(UnitPositionSetIterator* thisptr);
    Preincrement preinc;

    using FindByPosition = UnitPositionPair*(__stdcall*)(UnitPositionPair* value,
                                                         const UnitPositionSet* set,
                                                         int unitPosition);
    FindByPosition findByPosition;

    using HasNegativePosition = bool(__stdcall*)(const UnitPositionSet* thisptr);
    HasNegativePosition hasNegativePosition;
};

Api& get();

} // namespace UnitPositionSetApi

} // namespace game

#endif // UNITPOSITIONSET_H
