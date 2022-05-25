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

#ifndef MIDGARDMAPBLOCK_H
#define MIDGARDMAPBLOCK_H

#include "categoryids.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "mqpoint.h"
#include "terraincountmap.h"
#include <cstdint>

namespace game {

struct IMidgardObjectMap;
struct IMidgardStreamEnv;
struct LGroundCategory;
struct LTerrainCategory;

/*
 Tile contents:
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |  forest index 2 |    forest index    |                 |   tile variation   | ground |terrain |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
 */

/** Represents block of tiles in scenario file and game. */
struct CMidgardMapBlock : public IMidScenarioObject
{
    std::uint32_t tiles[32]; /**< 8 tiles along the X axis by 4 tiles along the Y axis. */
    CMqPoint position;
};

static_assert(sizeof(CMidgardMapBlock) == 144,
              "Size of CMidgardMapBlock structure must be exactly 144 bytes");

static constexpr inline TerrainId tileTerrain(std::uint32_t tile)
{
    return static_cast<TerrainId>(tile & 7);
}

static constexpr inline GroundId tileGround(std::uint32_t tile)
{
    return static_cast<GroundId>((tile >> 3) & 7);
}

static constexpr inline std::uint8_t tileForestIndex2(std::uint32_t tile)
{
    return tile >> 26;
}

namespace CMidgardMapBlockApi {

struct Api
{
    /** Allocates memory for map block object and calls constructor. */
    using Allocate = CMidgardMapBlock*(__stdcall*)(const CMidgardID* blockId);
    Allocate allocate;

    using Constructor = CMidgardMapBlock*(__thiscall*)(CMidgardMapBlock* thisptr,
                                                       const CMidgardID* blockId);
    Constructor constructor;

    /** Counts number of tiles with plain ground and their terrain coverage for each race. */
    using CountTerrainCoverage = bool(__thiscall*)(const CMidgardMapBlock* thisptr,
                                                   TerrainCountMap* terrainCoverage,
                                                   int* plainTiles);
    CountTerrainCoverage countTerrainCoverage;

    /** Sets terrain for tile at specified map position. */
    using SetTerrain = bool(__thiscall*)(CMidgardMapBlock* thisptr,
                                         const LTerrainCategory* terrain,
                                         const CMqPoint* position);
    SetTerrain setTerrain;

    /** Returns terrain for tile at specified position. */
    using GetTerrain = bool(__thiscall*)(CMidgardMapBlock* thisptr,
                                         LTerrainCategory* terrain,
                                         const CMqPoint* position);
    GetTerrain getTerrain;

    /** Sets ground for tile at specified map position. */
    using SetGround = bool(__thiscall*)(CMidgardMapBlock* thisptr,
                                        const LGroundCategory* ground,
                                        const CMqPoint* position);
    SetGround setGround;

    /** Returns ground for tile at specified position. */
    using GetGround = bool(__thiscall*)(CMidgardMapBlock* thisptr,
                                        LGroundCategory* ground,
                                        const CMqPoint* position);
    GetGround getGround;
};

Api& get();

IMidScenarioObjectVftable* vftable();

} // namespace CMidgardMapBlockApi

} // namespace game

#endif // MIDGARDMAPBLOCK_H
