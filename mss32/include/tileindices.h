/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef TILEINDICES_H
#define TILEINDICES_H

#include "tileprefixes.h"
#include <cstdint>

namespace game {

struct LTerrainCategory;
struct LGroundCategory;

/**
 * Indices of CIsoEngineGroundData::terrainTiles elements and their relations with tile images.
 * Order of these indices determines drawing order for border tiles.
 * Water tiles are drawn first staying at the bottom while ground tiles are drawn above.
 * Snowy terrain of mountain clans always drawn last and stays on top of others.
 */
enum class TileArrayIndex : std::uint32_t
{
    Black, /**< Black tiles drawn outside of map bounds. */
    Water,
    Neutral,
    Heretic,
    Undead,
    Human,
    Elf,
    Dwarf,
};

/** Stores indices of CIsoEngineGroundData::terrainTiles for fast access. */
struct TileIndices
{
    TileArrayIndex water;
    TileArrayIndex neutral;
    TileArrayIndex human;
    TileArrayIndex heretic;
    TileArrayIndex dwarf;
    TileArrayIndex undead;
    TileArrayIndex black;
    TileArrayIndex elf;
};

static_assert(sizeof(TileIndices) == 32, "Size of TileIndices structure must be exactly 32 bytes");

namespace TileIndicesApi {

struct Api
{
    using Constructor = TileIndices*(__thiscall*)(TileIndices* thisptr);
    Constructor constructor;

    using CreateBorderTiles = void(__stdcall*)(TileArrayIndex tileIndex, TilePrefix tilePrefix);
    CreateBorderTiles createBorderTiles;

    using CreateTerrainTiles = void(__stdcall*)(TileArrayIndex tileIndex, TilePrefix tilePrefix);
    CreateTerrainTiles createTerrainTiles;

    using GetTileDataIndex = int(__thiscall*)(const TileIndices* thisptr,
                                              const LGroundCategory* ground,
                                              const LTerrainCategory* terrain);
    GetTileDataIndex getTileDataIndex;
};

Api& get();

} // namespace TileIndicesApi

} // namespace game

#endif // TILEINDICES_H
