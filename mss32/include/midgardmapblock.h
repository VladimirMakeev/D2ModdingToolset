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
#include <cstdint>

namespace game {

struct IMidgardObjectMap;
struct IMidgardStreamEnv;

/** Represents block of tiles in scenario file and game. */
struct CMidgardMapBlock : public IMidScenarioObject
{
    CMidgardID blockId;
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

static constexpr inline std::uint8_t tileForestImage(std::uint32_t tile)
{
    return tile >> 26;
}

namespace CMidgardMapBlockApi {

struct Api
{
    /** Reads or writes object data. */
    using Stream = void(__thiscall*)(CMidgardMapBlock* thisptr,
                                     IMidgardObjectMap* objectMap,
                                     IMidgardStreamEnv* streamEnv);
    Stream stream;
};

Api& get();

} // namespace CMidgardMapBlockApi

} // namespace game

#endif // MIDGARDMAPBLOCK_H
