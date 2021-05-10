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

#ifndef TERRAINCOUNTLIST_H
#define TERRAINCOUNTLIST_H

#include "sortedlist.h"
#include "terraincat.h"

namespace game {

struct TerrainCount
{
    LTerrainCategory terrain;
    int tilesCount;
};

static_assert(sizeof(TerrainCount) == 16,
              "Size of TerrainCount structure must be exactly 16 bytes");

using TerrainCountList = SortedList<TerrainCount>;

namespace TerrainCountListApi {

struct Api
{
    /** Returns pointer to TerrainCount::tilesCount found by specified terrain category. */
    using GetTilesCount = int*(__thiscall*)(TerrainCountList thisptr,
                                            const LTerrainCategory* terrain);
    GetTilesCount getTilesCount;
};

Api& get();

} // namespace TerrainCountListApi

} // namespace game

#endif // TERRAINCOUNTLIST_H
