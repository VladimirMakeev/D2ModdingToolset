/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#ifndef SCENARIODATA_H
#define SCENARIODATA_H

#include "d2string.h"

namespace game {

/**
 * Holds brief information about scenario file registered in game.
 * Upon creating one of the base menu classes game checks all
 * scenario files in 'Exports' folder, verifies their headers
 * and registers them using this structure.
 */
struct ScenarioData
{
    String filePath; /**< Full path to scenario file. */
    String author;
    bool official;
    char padding[3];
    int* ptr;
    String description;
    String name;
    int mapSize;
    void* difficultyLevelVftable;
    int unknown1;
    int unknown2;
    int unknown3;
    int unknown4;
    int unknown5;
    int unknown6;
    int unknown7;
    int unknown8;
    void* raceCategoryVftable;
    int unknown9;
    int unknown10;
    int unknown11;
};

static_assert(sizeof(ScenarioData) == 128,
              "Size of ScenarioData structure must be exactly 128 bytes");

namespace ScenarioDataApi {

struct Api
{
    /** Creates empty ScenarioData. */
    using Constructor = ScenarioData*(__thiscall*)(ScenarioData* thisptr);
    Constructor constructor;

    /** Frees memory allocated for ScenarioData fields. */
    using Destructor = void*(__thiscall*)(ScenarioData* thisptr);
    Destructor destructor;

    /** Performs a deep copy of other into thisptr. */
    using CopyConstructor = ScenarioData*(__thiscall*)(ScenarioData* thisptr,
                                                       const ScenarioData* other);
    CopyConstructor copyConstructor;
};

Api& get();

} // namespace ScenarioDataApi

} // namespace game

#endif // SCENARIODATA_H
