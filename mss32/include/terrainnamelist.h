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

#ifndef TERRAINNAMELIST_H
#define TERRAINNAMELIST_H

#include "d2pair.h"
#include "sortedlist.h"
#include "terraincat.h"

namespace game {

using TerrainNamePair = Pair<LTerrainCategory, char[3]>;

/** Holds terrain categories and their abbreviations. */
using TerrainNameList = SortedList<TerrainNamePair>;
using TerrainNameListNode = SortedListNode<TerrainNamePair>;

static_assert(sizeof(TerrainNameListNode) == 32,
              "Size of TerrainNameListNode structure must be exactly 32 bytes");

struct TerrainNameListIterator
{
    char unknown[16];
};

namespace TerrainNameListApi {

struct Api
{
    /**
     * Returns terrain name list.
     * Initializes list on the first access.
     */
    using GetTerrainNameList = TerrainNameList* (*)();
    GetTerrainNameList getTerrainNameList;

    /** Adds new node to the list with specified terrain and abbreviation. */
    using Add = TerrainNameListIterator*(__thiscall*)(TerrainNameList* thisptr,
                                                      TerrainNameListIterator* iterator,
                                                      const LTerrainCategory* terrain,
                                                      const char* abbreviation);
    Add add;
};

Api& get();

} // namespace TerrainNameListApi

} // namespace game

#endif // TERRAINNAMELIST_H
