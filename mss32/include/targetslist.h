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

#include "sortedlist.h"

namespace game {

struct IMidgardObjectMap;
struct BattleMsgData;
struct IBatAttack;
struct CMidgardID;

/**
 * List of targets to attack during single battle turn.
 * Contains units positions in groups.
 * Position values are positive for allied units and negative for enemies.
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
    using Destructor = TargetsList*(__thiscall*)(TargetsList* thisptr);
    Destructor destructor;

    /**
     * Fills targets list for a specified attack.
     * @param[in] objectMap map where to search for objects.
     * @param[in] battleMsgData battle information.
     * @param[in] attack battle attack for which targets list will be filled.
     * @param[in] stackId id of stack whose unit performing the attack.
     * @param[in] id unit or item id performing the attack.
     * @param allies specifies whether the attack should target allies or not.
     * @param[inout] targetsList list to fill.
     * @param checkTransformed specifies whether the attack should check units transformations or
     * not.
     */
    using Fill = void(__stdcall*)(IMidgardObjectMap* objectMap,
                                  BattleMsgData* battleMsgData,
                                  IBatAttack* attack,
                                  CMidgardID* stackId,
                                  CMidgardID* id,
                                  bool allies,
                                  TargetsList* targetsList,
                                  bool checkTransformed);
    Fill fill;

    /** Clears list contents. */
    using Clear = void(__thiscall*)(TargetsList* thisptr);
    Clear clear;

    /** Returns iterator pointing to the first element in the list. */
    using Begin = TargetsListIterator*(__thiscall*)(TargetsList* thisptr,
                                                    TargetsListIterator* iterator);
    Begin begin;

    /** Inserts new element to the list. */
    using Insert = TargetsListIterator*(__thiscall*)(TargetsList* thisptr,
                                                     TargetsListIterator* iterator,
                                                     int* unitPosition);
    Insert insert;

    /** Removes existing element from list. */
    using Remove = void(__thiscall*)(TargetsList* thisptr, int* unitPosition);
    Remove remove;

    /**
     * Returns pointer to list node value depending on iterator.
     * Same as @code{.cpp} thisptr->node.value; @endcode
     */
    using Get = int*(__thiscall*)(TargetsListIterator* thisptr);
    Get get;
};

Api& get();

} // namespace TargetsListApi

} // namespace game

#endif // TARGETSLIST_H
