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

#ifndef TERRAINNAMEMAP_H
#define TERRAINNAMEMAP_H

#include "d2map.h"
#include "terraincat.h"

namespace game {

/** Holds terrain categories and their abbreviations. */
using TerrainNameMap = Map<LTerrainCategory, char[3]>;
using TerrainNameMapNode = MapNode<LTerrainCategory, char[3]>;
using TerrainNameMapIterator = MapIterator<LTerrainCategory, char[3]>;

assert_size(TerrainNameMapNode, 32);

namespace TerrainNameMapApi {

struct Api
{
    /**
     * Returns terrain name map.
     * Initializes list on the first access.
     */
    using Get = TerrainNameMap* (*)();
    Get get;

    /** Adds new node to the list with specified terrain and abbreviation. */
    using Add = TerrainNameMapIterator*(__thiscall*)(TerrainNameMap* thisptr,
                                                     TerrainNameMapIterator* iterator,
                                                     const LTerrainCategory* terrain,
                                                     const char* abbreviation);
    Add add;
};

Api& get();

} // namespace TerrainNameMapApi

} // namespace game

#endif // TERRAINNAMEMAP_H
