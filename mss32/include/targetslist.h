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

#ifndef TARGETSLIST_H
#define TARGETSLIST_H

#include "d2pair.h"
#include "sortedlist.h"

namespace game {

struct IMidgardObjectMap;
struct BattleMsgData;
struct IBatAttack;
struct CMidgardID;

/**
 * List of targets to attack during single battle turn.
 * Contains units positions in groups.
 * Position values are positive for target group and negative for other group.
 * Negative position computed as -(unitPosition + 1).
 */
using TargetsList = SortedList<int>;

using TargetsListNode = SortedListNode<int>;
using TargetsListIterator = SortedListIterator<int>;

namespace TargetsListApi {

struct Api
{
    /** Performs targets list initialization. */
    using Constructor = TargetsList*(__thiscall*)(TargetsList* thisptr);
    Constructor constructor;

    /** Frees memory allocated for list. */
    using Destructor = void(__thiscall*)(TargetsList* thisptr);
    Destructor destructor;

    /** Clears list contents. */
    using Clear = void(__thiscall*)(TargetsList* thisptr);
    Clear clear;

    /** Returns iterator pointing to the first element in the list. */
    using GetIterator = TargetsListIterator*(__thiscall*)(const TargetsList* thisptr,
                                                          TargetsListIterator* iterator);
    GetIterator begin;
    GetIterator end;

    /** Inserts new element to the list. */
    using Insert =
        Pair<TargetsListIterator, bool>*(__thiscall*)(TargetsList* thisptr,
                                                      Pair<TargetsListIterator, bool>* iterator,
                                                      int* unitPosition);
    Insert insert;

    /** Removes existing element from list. */
    using Erase = void(__thiscall*)(TargetsList* thisptr, int* unitPosition);
    Erase erase;

    /**
     * Returns pointer to list node value depending on iterator.
     * Same as @code{.cpp} thisptr->node.value; @endcode
     */
    using Dereference = int*(__thiscall*)(const TargetsListIterator* thisptr);
    Dereference dereference;

    using Equals = bool(__thiscall*)(const TargetsListIterator* thisptr,
                                     const TargetsListIterator* value);
    Equals equals;

    using Preincrement = TargetsListIterator*(__thiscall*)(TargetsListIterator* thisptr);
    Preincrement preinc;
};

Api& get();

} // namespace TargetsListApi

} // namespace game

#endif // TARGETSLIST_H
