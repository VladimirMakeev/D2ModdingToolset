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

#include "tileindiceshooks.h"
#include "groundcat.h"
#include "log.h"
#include "mapgraphics.h"
#include "mempool.h"
#include "terrainhooks.h"
#include "tileindices.h"
#include <fmt/format.h>
#include <vector>

namespace hooks {

/**
 * Tile indices for custom terrains.
 * Sequence of elements here must match with the sequence in customTerrains().
 */
static std::vector<int> customTileIndices;

game::TileIndices* __fastcall tileIndicesCtorHooked(game::TileIndices* thisptr, int /*%edx*/)
{
    using namespace game;

    logDebug("newTerrain.log", "Init extended tile data");

    const auto& tileIndices = TileIndicesApi::get();
    const auto& mapGraphics = MapGraphicsApi::get();

    MapGraphicsPtr mapGraphicsPtr{};
    mapGraphics.getMapGraphics(&mapGraphicsPtr);

    auto graphics{mapGraphicsPtr.data};

    thisptr->black = mapGraphics.getTileIndex(graphics, false);
    logDebug("newTerrain.log", fmt::format("Black value {:d}, expect 0", thisptr->black));

    thisptr->water = mapGraphics.getTileIndex(graphics, true);
    logDebug("newTerrain.log", fmt::format("Water value {:d}, expect 1", thisptr->water));

    thisptr->neutral = mapGraphics.getTileIndex(graphics, false);
    logDebug("newTerrain.log", fmt::format("Neutral value {:d}, expect 2", thisptr->neutral));

    thisptr->heretic = mapGraphics.getTileIndex(graphics, false);
    logDebug("newTerrain.log", fmt::format("Heretic value {:d}, expect 3", thisptr->heretic));

    thisptr->undead = mapGraphics.getTileIndex(graphics, false);
    logDebug("newTerrain.log", fmt::format("Undead value {:d}, expect 4", thisptr->undead));

    thisptr->human = mapGraphics.getTileIndex(graphics, false);
    logDebug("newTerrain.log", fmt::format("Human value {:d}, expect 5", thisptr->human));

    thisptr->elf = mapGraphics.getTileIndex(graphics, false);
    logDebug("newTerrain.log", fmt::format("Elf value {:d}, expect 6", thisptr->elf));

    thisptr->dwarf = mapGraphics.getTileIndex(graphics, false);
    logDebug("newTerrain.log", fmt::format("Dwarf value {:d}, expect 7", thisptr->dwarf));

    customTileIndices.resize(customTerrains().size());
    int i{};
    for (auto& customIndex : customTileIndices) {
        customIndex = mapGraphics.getTileIndex(graphics, false);
        logDebug("newTerrain.log",
                 fmt::format("Custom value {:d}, expect {:d}", customIndex, 8 + i));
        i++;
    }

    logDebug("newTerrain.log", "Store black tiles");
    mapGraphics.storeBlackTiles(graphics, thisptr->black);

    logDebug("newTerrain.log", "Create neutral border tiles");
    tileIndices.createBorderTiles(thisptr->neutral, 1);

    logDebug("newTerrain.log", "Create human border tiles");
    tileIndices.createBorderTiles(thisptr->human, 1);

    logDebug("newTerrain.log", "Create heretic border tiles");
    tileIndices.createBorderTiles(thisptr->heretic, 1);

    logDebug("newTerrain.log", "Create dwarf border tiles");
    tileIndices.createBorderTiles(thisptr->dwarf, 1);

    logDebug("newTerrain.log", "Create undead border tiles");
    tileIndices.createBorderTiles(thisptr->undead, 1);

    logDebug("newTerrain.log", "Create elf border tiles");
    tileIndices.createBorderTiles(thisptr->elf, 1);

    for (const auto& customIndex : customTileIndices) {
        logDebug("newTerrain.log", "Create custom border tiles");
        tileIndices.createBorderTiles(customIndex, 1);
    }

    logDebug("newTerrain.log", "Create water border tiles");
    tileIndices.createBorderTiles(thisptr->water, 0);

    logDebug("newTerrain.log", "Create black terrain tiles");
    tileIndices.createTerrainTiles(thisptr->black, 6);

    logDebug("newTerrain.log", "Create neutral terrain tiles");
    tileIndices.createTerrainTiles(thisptr->neutral, 1);

    logDebug("newTerrain.log", "Create human terrain tiles");
    tileIndices.createTerrainTiles(thisptr->human, 2);

    logDebug("newTerrain.log", "Create heretic terrain tiles");
    tileIndices.createTerrainTiles(thisptr->heretic, 3);

    logDebug("newTerrain.log", "Create dwarf terrain tiles");
    tileIndices.createTerrainTiles(thisptr->dwarf, 4);

    logDebug("newTerrain.log", "Create undead terrain tiles");
    tileIndices.createTerrainTiles(thisptr->undead, 5);

    logDebug("newTerrain.log", "Create elf terrain tiles");
    tileIndices.createTerrainTiles(thisptr->elf, 7);

    for (std::size_t j = 0; j < customTileIndices.size(); ++j) {
        logDebug("newTerrain.log", "Create custom terrain tiles");
        tileIndices.createTerrainTiles(customTileIndices[j], 8 + j);
    }

    mapGraphics.setMapGraphics(&mapGraphicsPtr, nullptr);

    return thisptr;
}

int __fastcall getTileDataIndexHooked(const game::TileIndices* thisptr,
                                      int /*%edx*/,
                                      const game::LGroundCategory* ground,
                                      const game::LTerrainCategory* terrain)
{
    using namespace game;

    /*logDebug("newTerrain.log",
             fmt::format("Get tile data index hook started, terrainId {:d}", (int)terrain->id));*/

    const auto& groundCategories = GroundCategories::get();

    if (groundCategories.water->id == ground->id) {
        // logDebug("newTerrain.log", "Get water tile data index");
        return thisptr->water;
    }

    if (groundCategories.mountain->id == ground->id) {
        // logDebug("newTerrain.log", "Get neutral (mountain) tile data index");
        return thisptr->neutral;
    }

    const auto& terrainCategories = TerrainCategories::get();
    const auto terrainId{terrain->id};

    if (terrainCategories.neutral->id == terrainId) {
        // logDebug("newTerrain.log", "Get neutral tile data index");
        return thisptr->neutral;
    }

    if (terrainCategories.human->id == terrainId) {
        // logDebug("newTerrain.log", "Get human tile data index");
        return thisptr->human;
    }

    if (terrainCategories.heretic->id == terrainId) {
        // logDebug("newTerrain.log", "Get heretic tile data index");
        return thisptr->heretic;
    }

    if (terrainCategories.dwarf->id == terrainId) {
        // logDebug("newTerrain.log", "Get dwarf tile data index");
        return thisptr->dwarf;
    }

    if (terrainCategories.undead->id == terrainId) {
        // logDebug("newTerrain.log", "Get undead tile data index");
        return thisptr->undead;
    }

    if (terrainCategories.elf->id == terrainId) {
        // logDebug("newTerrain.log", "Get elf tile data index");
        return thisptr->elf;
    }

    std::size_t i{};
    for (const auto& customTerrain : customTerrains()) {
        if (customTerrain.category.id == terrainId) {
            return customTileIndices[i];
        }

        i++;
    }

    // logDebug("newTerrain.log", "Get tile data index hook. Return neutral as safe option");
    return thisptr->neutral;
}

} // namespace hooks
