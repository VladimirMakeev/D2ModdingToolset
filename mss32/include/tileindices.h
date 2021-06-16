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

#ifndef TILEINDICES_H
#define TILEINDICES_H

namespace game {

struct LTerrainCategory;
struct LGroundCategory;

/** Indices for fast CIsoEngineGround data lookup. */
struct TileIndices
{
    int water;
    int neutral;
    int human;
    int heretic;
    int dwarf;
    int undead;
    int black; /**< Fog of war. */
    int elf;
};

static_assert(sizeof(TileIndices) == 32, "Size of TileIndices structure must be exactly 32 bytes");

namespace TileIndicesApi {

struct Api
{
    using Constructor = TileIndices*(__thiscall*)(TileIndices* thisptr);
    Constructor constructor;

    using CreateBorderTiles = void(__stdcall*)(int tileIndex, int terrainTile);
    CreateBorderTiles createBorderTiles;

    using CreateTerrainTiles = void(__stdcall*)(int tileIndex, int tilePrefixNumber);
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
