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
#include "midgardobjectmap.h"
#include <cstdint>

namespace game {

struct ScenarioObjectRecord
{
    CMidgardID objectId;
    SmartPtr<IMidScenarioObject> object;
    std::uint32_t partialId; /**< objectId.value & 0x7fffffff */
    ScenarioObjectRecord* next;
};

assert_size(ScenarioObjectRecord, 20);

struct CMidgardScenarioMapData
{
    char unknown;
    char padding[3];
    std::uint32_t objectsTotal;
    ScenarioObjectRecord** records;
    std::uint32_t recordsTotal;
    int unknown2;
    int unknown3;
    char unknown4;
    char unknown5;
    char unknown6;
    char unknown7;
    int unknown8;
    int unknown9;
    void* allocator;
};

assert_size(CMidgardScenarioMapData, 40);

/** Stores scenario objects. */
struct CMidgardScenarioMap : public IMidgardObjectMap
{
    CMidgardID scenarioFileId;
    CMidgardScenarioMapData data;
    /** Used to keep track of last free CMidgardID type index for each IdType. */
    int freeIdTypeIndices[59];
    Set<CMidgardID> addedObjects;
    Set<CMidgardID> changedObjects;
    Set<CMidgardID> objectsToErase;
};

assert_size(CMidgardScenarioMap, 368);

} // namespace game

#endif // MIDGARDSCENARIOMAP_H
