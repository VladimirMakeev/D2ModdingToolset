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

#ifndef MIDGARDOBJECTMAP_H
#define MIDGARDOBJECTMAP_H

#include "midgardid.h"
#include "smartptr.h"

namespace game {

struct IMidgardObjectMap;
struct IMidScenarioObject;

struct IMidgardObjectMapVftable
{
    using Destructor = void(__thiscall*)(IMidgardObjectMap* thisptr, char flags);
    Destructor destructor;

    /** Returns scenario id. */
    using GetId = const CMidgardID*(__thiscall*)(const IMidgardObjectMap* thisptr);
    GetId getId;

    /** Returns total number of objects in scenario. */
    using GetObjectsTotal = int(__thiscall*)(const IMidgardObjectMap* thisptr);
    GetObjectsTotal getObjectsTotal;

    using CreateIterator = SmartPointer*(__thiscall*)(IMidgardObjectMap* thisptr,
                                                      SmartPointer* iterator);
    CreateIterator createIterator;
    CreateIterator createIterator2;

    /**
     * Finds scenario object by its id.
     * Returns nullptr if object could not be found.
     */
    using FindScenarioObjectById =
        IMidScenarioObject*(__thiscall*)(const IMidgardObjectMap* thisptr, const CMidgardID* id);
    FindScenarioObjectById findScenarioObjectById;

    /**
     * Finds scenario object by its id and adds it to change list.
     * Returns nullptr if object could not be found.
     */
    using FindScenarioObjectByIdForChange =
        IMidScenarioObject*(__thiscall*)(IMidgardObjectMap* thisptr, const CMidgardID* id);
    FindScenarioObjectByIdForChange findScenarioObjectByIdForChange;

    /**
     * Finds scenario object by its id and adds it to erase list.
     * Deletes object record locally.
     * Returns true if object was deleted.
     */
    using DeleteScenarioObjectById = bool(__thiscall*)(IMidgardObjectMap* thisptr,
                                                       const CMidgardID* id);
    DeleteScenarioObjectById deleteScenarioObjectById;

    /**
     * Adds scenario object to the map.
     * Updates list of added objects.
     * Returns true if object was added.
     */
    using InsertObject = bool(__thiscall*)(IMidgardObjectMap* thisptr,
                                           const IMidScenarioObject* object);
    InsertObject insertObject;

    using CreateScenarioId = void(__thiscall*)(IMidgardObjectMap* thisptr,
                                               CMidgardID* scenarioId,
                                               IdType type);

    /**
     * Creates id with IdCategory::Scenario and specified type.
     * Increments free type index for specified IdType.
     */
    CreateScenarioId createScenarioIdIncFreeIndex;

    /** Creates id with IdCategory::Scenario and specified type. */
    CreateScenarioId createScenarioId;

    /** Sets free type index for specified IdType. */
    using SetFreeTypeIndex = void(__thiscall*)(IMidgardObjectMap* thisptr,
                                               IdType type,
                                               int freeTypeIndex);
    SetFreeTypeIndex setFreeTypeIndex;

    void* unknown;
};

static_assert(sizeof(IMidgardObjectMapVftable) == 13 * sizeof(void*),
              "IMidgardObjectMap vftable must have exactly 13 methods");

struct IMidgardObjectMap
{
    struct IteratorVftable;

    struct Iterator
    {
        IteratorVftable* vftable;
        SmartPointer iterator;
        IMidgardObjectMap* objectMap;
    };

    struct IteratorVftable
    {
        using Destructor = void(__thiscall*)(Iterator* thisptr, char flags);
        Destructor destructor;

        /** Returns true if iterator reached end. */
        using End = bool(__thiscall*)(Iterator* thisptr, Iterator* endIterator);
        End end;

        /** Returns id of the object that the iterator is currently pointing. */
        using GetObjectId = CMidgardID*(__thiscall*)(Iterator* thisptr);
        GetObjectId getObjectId;

        /** Advances iterator so it starts pointing to the next object or end. */
        using Advance = void(__thiscall*)(Iterator* thisptr);
        Advance advance;
    };

    const IMidgardObjectMapVftable* vftable;
};

static_assert(sizeof(IMidgardObjectMap::Iterator) == 16,
              "IMidgardObjectMap::Iterator structure must be exactly 16 bytes");

static_assert(sizeof(IMidgardObjectMap::IteratorVftable) == 4 * sizeof(void*),
              "IMidgardObjectMap::Iterator vftable must have exactly 4 methods");

} // namespace game

#endif // MIDGARDOBJECTMAP_H
