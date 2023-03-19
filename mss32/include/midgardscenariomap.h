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

#ifndef MIDGARDSCENARIOMAP_H
#define MIDGARDSCENARIOMAP_H

#include "d2set.h"
#include "d2unorderedmap.h"
#include "midgardobjectmap.h"
#include <cstdint>

namespace game {

struct IMidgardStreamEnv;

using ScenarioObjectRecord = UnorderedMapBucket<CMidgardID, SmartPtr<IMidScenarioObject>>;
assert_size(ScenarioObjectRecord, 20);

/** Stores scenario objects. */
struct CMidgardScenarioMap : public IMidgardObjectMap
{
    CMidgardID scenarioFileId;
    UnorderedMap<CMidgardID, SmartPtr<IMidScenarioObject>> data;
    /** Used to keep track of last free CMidgardID type index for each IdType. */
    int freeIdTypeIndices[59];
    Set<CMidgardID> addedObjects;
    Set<CMidgardID> changedObjects;
    Set<CMidgardID> objectsToErase;
};

assert_size(CMidgardScenarioMap, 368);

struct ScenarioMapDataIterator
{
    bool unknown;
    char padding[3];
    ScenarioObjectRecord** records;
    ScenarioObjectRecord* foundRecord;
    int recordsTotal;
};

assert_size(ScenarioMapDataIterator, 16);

struct CMidgardScenarioMapIter : public IMidgardObjectMap::Iterator
{
    ScenarioMapDataIterator iterator;
};

assert_size(CMidgardScenarioMapIter, 20);

namespace CMidgardScenarioMapApi {

struct Api
{
    using GetIterator = ScenarioMapDataIterator*(__thiscall*)(CMidgardScenarioMap* thisptr,
                                                              ScenarioMapDataIterator* iterator);

    /** Returns a raw iterator to the first record. */
    GetIterator begin;

    /** Returns a raw iterator to the record following the last one. */
    GetIterator end;

    /** Advances raw iterator. */
    using Advance = void(__thiscall*)(ScenarioMapDataIterator* thisptr);
    Advance advance;

    /** Checks objects correctness and updates freeIdTypeIndices. */
    using CheckObjects = bool(__thiscall*)(CMidgardScenarioMap* scenarioMap);
    CheckObjects checkObjects;

    using Stream = bool(__thiscall*)(CMidgardScenarioMap* scenarioMap,
                                     IMidgardStreamEnv* streamEnv);
    Stream stream;
};

Api& get();

IMidgardObjectMapVftable* vftable();

} // namespace CMidgardScenarioMapApi

} // namespace game

#endif // MIDGARDSCENARIOMAP_H
