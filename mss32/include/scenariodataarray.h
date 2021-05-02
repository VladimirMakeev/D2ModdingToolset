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

#ifndef SCENARIODATAARRAY_H
#define SCENARIODATAARRAY_H

#include "d2vector.h"

namespace game {

struct ScenarioData;

using ScenarioDataArray = Vector<ScenarioData>;

struct ScenarioDataArrayWrapped
{
    bool initialized;
    char padding[3];
    ScenarioDataArray data;
    int maxScenarioFileIndex;
};

static_assert(sizeof(ScenarioDataArrayWrapped) == 24,
              "Size of ScenarioDataArrayWrapped structure must be exactly 24 bytes");

namespace ScenarioDataArrayApi {

struct Api
{
    /**
     * Adds new ScenarioData element to the end of the array.
     * @param[in] thisptr pointer to array to add to.
     * @param[in] data element to push.
     * @returns unknown.
     */
    using PushBack = void*(__thiscall*)(ScenarioDataArray* thisptr, const ScenarioData* data);
    PushBack pushBack;

    /** Returns element at specified index. */
    using GetElement = ScenarioData*(__thiscall*)(ScenarioDataArrayWrapped* thisptr, int index);
    GetElement getElement;
};

Api& get();

} // namespace ScenarioDataArrayApi

} // namespace game

#endif // SCENARIODATAARRAY_H
