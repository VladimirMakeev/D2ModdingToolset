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

#ifndef MIDEVCONDITION_H
#define MIDEVCONDITION_H

#include "eventconditioncat.h"
#include "midgardid.h"
#include "sortedlist.h"

namespace game {

struct CMidEvConditionVftable;
struct IMidgardObjectMap;
struct IMidgardStream;
struct String;

/** Base class for all event conditions. */
struct CMidEvCondition
{
    CMidEvConditionVftable* vftable;
    LEventCondCategory category;
};

struct CMidEvConditionVftable
{
    using Destructor = void(__thiscall*)(CMidEvCondition* thisptr, char flags);
    Destructor destructor;

    /**
     * Returns true if id used by event condition equals to the objectId specified.
     * Used by CMidCondLocation, CMidCondEnterCity, CMidCondKillStack, CMidCondOwnItem etc.
     */
    using IsIdsEqual = bool(__thiscall*)(const CMidEvCondition* thisptr,
                                         const CMidgardID* objectId);
    IsIdsEqual isIdsEqual;

    /**
     * Adds id used by event to the list.
     * Used by CMidCondLocation, CMidCondEnterCity, CMidCondKillStack, CMidCondOwnItem etc.
     */
    using AddToList = void(__thiscall*)(const CMidEvCondition* thisptr,
                                        SortedList<CMidgardID>* list);
    AddToList addToList;

    /** Returns true if condition is valid: refers to existing scenario objects. */
    using IsValid = bool(__thiscall*)(const CMidEvCondition* thisptr,
                                      const IMidgardObjectMap* objectMap);
    IsValid isValid;

    using Method4 = bool(__thiscall*)(const CMidEvCondition* thisptr, int a2);
    Method4 method4;

    /** Serializes event condition. */
    using Stream = void(__thiscall*)(CMidEvCondition* thisptr, IMidgardStream** stream);
    Stream stream;
};

static_assert(sizeof(CMidEvConditionVftable) == 6 * sizeof(void*),
              "CMidEvCondition vftable must have exactly 6 methods");

namespace CMidEvConditionApi {

struct Api
{
    using CreateFromCategory = CMidEvCondition*(__stdcall*)(const LEventCondCategory* category);
    CreateFromCategory createFromCategory;

    /**
     * Returns string with current condition settings to show in conditions list box.
     * Used in Scenario Editor only.
     */
    using GetInfoString = void(__stdcall*)(String* info,
                                           const IMidgardObjectMap* objectMap,
                                           const CMidEvCondition* eventCondition);
    GetInfoString getInfoString;

    using GetDescription = void(__stdcall*)(String* description,
                                            const LEventCondCategory* category);

    /**
     * Returns brief condition description by specified category
     * Used in Scenario Editor only.
     */
    GetDescription getBrief;

    /**
     * Returns full condition description by specified category
     * Used in Scenario Editor only.
     */
    GetDescription getDescription;
};

Api& get();

} // namespace CMidEvConditionApi

} // namespace game

#endif // MIDEVCONDITION_H
