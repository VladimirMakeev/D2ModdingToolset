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

#ifndef PATHINFOLIST_H
#define PATHINFOLIST_H

#include "d2list.h"
#include "mqpoint.h"

namespace game {

struct IMidgardObjectMap;
struct CMidStack;

/** Holds data about single point along the path. */
struct PathInfo
{
    CMqPoint position; /**< Tile coordinates. */
    int moveCostTotal; /**< Total cost to reach this point from the start of the path. */
    int turnsToReach;  /**< Number of turns the stack needs to reach this point. */
};

/** Used to show movement path hints to the player in strategic view. */
using PathInfoList = List<PathInfo>;
using PathInfoListNode = ListNode<PathInfo>;

namespace PathInfoListApi {

struct Api
{
    using Constructor = PathInfoList*(__thiscall*)(PathInfoList* thisptr);
    Constructor constructor;

    /**
     * Fills list with actual movement cost and turns information.
     * @param[in] objectMap map where to search for objects.
     * @param[in] stack stack for which to compute movement cost and turns information.
     * @param[in] path movement path.
     * @param[in] lastReachablePoint last point of the path that stack can reach in single turn.
     * @param waterOnly treat stack leader as water only unit or not.
     * @param[inout] pathInfo path information list to fill.
     */
    using PopulateFromPath = void(__stdcall*)(const IMidgardObjectMap* objectMap,
                                              const CMidStack* stack,
                                              const List<CMqPoint>* path,
                                              const CMqPoint* lastReachablePoint,
                                              bool waterOnly,
                                              PathInfoList* pathInfo);
    PopulateFromPath populateFromPath;

    using FreeNodes = void(__thiscall*)(PathInfoList* thisptr);
    FreeNodes freeNodes;

    using FreeNode = void(__thiscall*)(PathInfoList* thisptr, PathInfoListNode* node);
    FreeNode freeNode;
};

Api& get();

} // namespace PathInfoListApi

} // namespace game

#endif // PATHINFOLIST_H
