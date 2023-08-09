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

#ifndef MIDUNITGROUP_H
#define MIDUNITGROUP_H

#include "idlist.h"
#include "idvector.h"
#include "unitpositionmap.h"

namespace game {

struct CMidUnitGroupVftable;
struct IMidgardObjectMap;
struct CFortification;
struct CVisitorAddUnitToGroup;

/** Represents group of 6 units in game. */
struct CMidUnitGroup
{
    const CMidUnitGroupVftable* vftable;
    IdVector units;
    CMidgardID positions[6];
    /**
     * Maximum number of units that can be added to the group.
     * Value of -1 disables restriction.
     * Does not work properly with units hire and used only for summon attacks logic.
     */
    int maxUnitsAllowed;
    CFortification* city;
};

struct CMidUnitGroupVftable
{
    using Destructor = void(__thiscall*)(CMidUnitGroup* thisptr, char flags);
    Destructor destructor;

    using AddLeader = bool(__thiscall*)(CMidUnitGroup* thisptr,
                                        const CMidgardID* unitId,
                                        int position,
                                        const IMidgardObjectMap* objectMap);
    AddLeader addLeader;

    using RemoveAllUnits = bool(__thiscall*)(CMidUnitGroup* thisptr,
                                             const CMidgardID* groupId,
                                             const IMidgardObjectMap* objectMap);
    RemoveAllUnits removeAllUnits;

    using Stream = void(__thiscall*)(CMidUnitGroup* thisptr, int a2, const CMidgardID* groupId);
    Stream stream;

    void* methods[2];

    using AddUnit = bool(__thiscall*)(CMidUnitGroup* thisptr,
                                      const CVisitorAddUnitToGroup* visitor,
                                      const CMidgardID* unitId,
                                      int position,
                                      const IMidgardObjectMap* objectMap);
    AddUnit addUnit;

    using RemoveUnit = bool(__thiscall*)(CMidUnitGroup* thisptr,
                                         CVisitorAddUnitToGroup* visitor,
                                         const CMidgardID* unitId,
                                         const IMidgardObjectMap* objectMap);
    RemoveUnit removeUnit;

    void* methods2[2];
};

assert_vftable_size(CMidUnitGroupVftable, 10);

namespace CMidUnitGroupApi {

struct Api
{
    using GetUnitId = CMidgardID*(__thiscall*)(const CMidUnitGroup* thisptr, int index);
    GetUnitId getUnitId;

    /** Returns unit position in group, or -1 if unit not found. */
    using GetUnitPosition = int(__thiscall*)(const CMidUnitGroup* thisptr,
                                             const CMidgardID* unitId);
    GetUnitPosition getUnitPosition;

    /** Returns unit id by specified position in group, or emptyId in case of wrong position. */
    using GetUnitIdByPosition = const CMidgardID*(__thiscall*)(const CMidUnitGroup* thisptr,
                                                               int position);
    GetUnitIdByPosition getUnitIdByPosition;

    using AddUnitIdsAvailableForSummons = void(__stdcall*)(IdList* value,
                                                           const IMidgardObjectMap* objectMap,
                                                           const CMidUnitGroup* group);
    AddUnitIdsAvailableForSummons addUnitIdsAvailableForSummons;

    using UnknownFunction = void(__stdcall*)(CMidUnitGroup* group1,
                                             CMidUnitGroup* group2,
                                             UnitPositionMap* positions);
    UnknownFunction unknownFunction;

    /** Returns number of units in group. */
    using GetUnitsCount = int(__thiscall*)(const CMidUnitGroup* group);
    GetUnitsCount getUnitsCount;
};

Api& get();

} // namespace CMidUnitGroupApi

} // namespace game

#endif // MIDUNITGROUP_H
