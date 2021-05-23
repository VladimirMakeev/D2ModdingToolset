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

#ifndef TERRAINHOOKS_H
#define TERRAINHOOKS_H

#include "terraincat.h"
#include <string>
#include <vector>

namespace game {
struct LRaceCategory;
struct LGroundCategory;
} // namespace game

namespace hooks {

struct TerrainDescription
{
    game::LTerrainCategory category{};
    std::string categoryName;
    std::string abbreviation;
};

using TerrainDescriptions = std::vector<TerrainDescription>;

TerrainDescriptions& customTerrains();

game::LTerrainCategoryTable* __fastcall terrainCategoryTableCtorHooked(
    game::LTerrainCategoryTable* thisptr,
    int /*%edx*/,
    const char* globalsFolderPath,
    void* codeBaseEnvProxy);

game::LTerrainCategory* __stdcall getTerrainByAbbreviationHooked(game::LTerrainCategory* category,
                                                                 const char* abbreviation);

const game::LTerrainCategory* __stdcall getTerrainByRaceHooked(const game::LRaceCategory* race);

const game::LRaceCategory* __stdcall getRaceByTerrainHooked(const game::LTerrainCategory* terrain);

const game::LRaceCategory* __stdcall getPlayableRaceByTerrainHooked(
    const game::LTerrainCategory* terrain);

int __stdcall getTilePrefixByNameHooked(const char* tileNamePrefix);

const char* __stdcall getTilePrefixNameHooked(int tilePrefixNumber);

int __stdcall getNumberByTerrainGroundHooked(const game::LTerrainCategory* terrain,
                                             const game::LGroundCategory* ground);

} // namespace hooks

#endif // TERRAINHOOKS_H
