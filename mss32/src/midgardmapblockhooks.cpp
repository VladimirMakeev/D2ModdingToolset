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

#include "midgardmapblockhooks.h"
#include "groundcat.h"
#include "terrainhooks.h"
#include <vector>

namespace hooks {

enum TerrainCoverageIndex
{
    HumanIndex,
    DwarfIndex,
    HereticIndex,
    UndeadIndex,
    NeutralIndex,
    ElfIndex,
    CustomIndex,
};

bool __fastcall countTerrainCoverageHooked(const game::CMidgardMapBlock* thisptr,
                                           int /*%edx*/,
                                           game::TerrainCountList* terrainCoverage,
                                           int* plainTiles)
{
    using namespace game;

    const auto plainsGroundId{GroundCategories::get().plain->id};
    const auto& terrainCategories = TerrainCategories::get();
    const auto& terrains = customTerrains();
    const auto tiles = thisptr->tiles;

    std::vector<int> coverage(6 + terrains.size(), 0);
    int plainTilesCount{};

    for (std::uint32_t i = 0; i < 32; ++i) {
        const auto tile{tiles[i]};

        if (tileGround(tile) != plainsGroundId) {
            continue;
        }

        plainTilesCount++;
        const auto terrain{tileTerrain(tile)};

        if (terrain == terrainCategories.human->id) {
            coverage[HumanIndex]++;
            continue;
        }

        if (terrain == terrainCategories.dwarf->id) {
            coverage[DwarfIndex]++;
            continue;
        }

        if (terrain == terrainCategories.heretic->id) {
            coverage[HereticIndex]++;
            continue;
        }

        if (terrain == terrainCategories.undead->id) {
            coverage[UndeadIndex]++;
            continue;
        }

        if (terrain == terrainCategories.neutral->id) {
            coverage[NeutralIndex]++;
            continue;
        }

        if (terrain == terrainCategories.elf->id) {
            coverage[ElfIndex]++;
            continue;
        }

        for (std::size_t j = 0; j < terrains.size(); ++j) {
            const auto& customTerrain = terrains[j];

            if (terrain == customTerrain.category.id) {
                coverage[CustomIndex + j]++;
                break;
            }
        }
    }

    *plainTiles += plainTilesCount;

    const auto& getTilesCount = TerrainCountListApi::get().getTilesCount;

    auto humanCoverage = getTilesCount(terrainCoverage, terrainCategories.human);
    *humanCoverage += coverage[HumanIndex];

    auto dwarfCoverage = getTilesCount(terrainCoverage, terrainCategories.dwarf);
    *dwarfCoverage += coverage[DwarfIndex];

    auto hereticCoverage = getTilesCount(terrainCoverage, terrainCategories.heretic);
    *hereticCoverage += coverage[HereticIndex];

    auto undeadCoverage = getTilesCount(terrainCoverage, terrainCategories.undead);
    *undeadCoverage += coverage[UndeadIndex];

    auto neutralCoverage = getTilesCount(terrainCoverage, terrainCategories.neutral);
    *neutralCoverage += coverage[NeutralIndex];

    auto elfCoverage = getTilesCount(terrainCoverage, terrainCategories.elf);
    *elfCoverage += coverage[ElfIndex];

    for (std::size_t i = 0; i < terrains.size(); ++i) {
        auto newTerrainCoverage = getTilesCount(terrainCoverage, &terrains[i].category);
        *newTerrainCoverage += coverage[CustomIndex + i];
    }

    return true;
}

} // namespace hooks
