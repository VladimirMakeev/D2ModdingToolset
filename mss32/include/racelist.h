/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#ifndef RACELIST_H
#define RACELIST_H

#include "linkedlist.h"
#include "racecategory.h"

namespace game {

using RaceCategoryList = LinkedList<LRaceCategory>;
using RaceCategoryListNode = LinkedListNode<LRaceCategory>;

namespace RaceCategoryListApi {

struct Api
{
    /** Constructs an empty list. */
    using Constructor = RaceCategoryList*(__thiscall*)(RaceCategoryList* thisptr);
    Constructor constructor;

    /** Copy constructs list from 'other'. */
    using CopyConstructor = RaceCategoryList*(__thiscall*)(RaceCategoryList* thisptr,
                                                           const RaceCategoryList* other);
    CopyConstructor copyConstructor;

    /** Frees memory allocated for list nodes. */
    using FreeNodes = void(__thiscall*)(RaceCategoryList* thisptr);
    FreeNodes freeNodes;

    /** Frees memory allocated for specified node. */
    using FreeNode = void(__thiscall*)(RaceCategoryList* thisptr, RaceCategoryListNode* node);
    FreeNode freeNode;

    /** Adds race category to the list. */
    using Add = void(__thiscall*)(RaceCategoryList* thisptr, const LRaceCategory* race);
    Add add;

    /** Returns list with playable race categories. */
    using GetPlayableRaces = RaceCategoryList*(__stdcall*)(RaceCategoryList* races);
    GetPlayableRaces getPlayableRaces;
};

Api& get();

} // namespace RaceCategoryListApi

} // namespace game

#endif // RACELIST_H
