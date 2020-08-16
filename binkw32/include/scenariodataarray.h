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

#ifndef SCENARIODATAARRAY_H
#define SCENARIODATAARRAY_H

namespace game {

struct ScenarioData;

struct ScenarioDataArray
{
    ScenarioData* bgn;
    ScenarioData* end;
    ScenarioData* allocatedMemEnd;
};

struct ScenarioDataArrayWrapped
{
    int initialized;
    ScenarioDataArray data;
};

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
