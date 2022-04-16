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

#ifndef TARGETSET_H
#define TARGETSET_H

#include "d2pair.h"
#include "d2set.h"

namespace game {

struct IMidgardObjectMap;
struct BattleMsgData;
struct IBatAttack;
struct CMidgardID;

/**
 * Set of targets to attack during single battle turn.
 * Contains units positions in groups.
 * Position values are positive for target group and negative for other group.
 * Negative position computed as -(unitPosition + 1).
 */
using TargetSet = Set<int>;
using TargetSetNode = SetNode<int>;
using TargetSetIterator = SetIterator<int>;

namespace TargetSetApi {

struct Api
{
    /** Performs targets list initialization. */
    using Constructor = TargetSet*(__thiscall*)(TargetSet* thisptr);
    Constructor constructor;

    /** Frees memory allocated for list. */
    using Destructor = void(__thiscall*)(TargetSet* thisptr);
    Destructor destructor;

    /** Clears list contents. */
    using Clear = void(__thiscall*)(TargetSet* thisptr);
    Clear clear;

    /** Returns iterator pointing to the first element in the list. */
    using GetIterator = TargetSetIterator*(__thiscall*)(const TargetSet* thisptr,
                                                        TargetSetIterator* iterator);
    GetIterator begin;
    GetIterator end;

    /** Inserts new element to the list. */
    using Insert =
        Pair<TargetSetIterator, bool>*(__thiscall*)(TargetSet* thisptr,
                                                    Pair<TargetSetIterator, bool>* iterator,
                                                    int* unitPosition);
    Insert insert;

    /** Removes existing element from list. */
    using Erase = void(__thiscall*)(TargetSet* thisptr, int* unitPosition);
    Erase erase;

    /**
     * Returns pointer to list node value depending on iterator.
     * Same as @code{.cpp} thisptr->node.value; @endcode
     */
    using Dereference = int*(__thiscall*)(const TargetSetIterator* thisptr);
    Dereference dereference;

    using Equals = bool(__thiscall*)(const TargetSetIterator* thisptr,
                                     const TargetSetIterator* value);
    Equals equals;

    using Preincrement = TargetSetIterator*(__thiscall*)(TargetSetIterator* thisptr);
    Preincrement preinc;
};

Api& get();

} // namespace TargetSetApi

} // namespace game

#endif // TARGETSET_H
