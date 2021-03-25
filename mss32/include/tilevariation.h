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

#ifndef TILEVARIATION_H
#define TILEVARIATION_H

#include "groundcat.h"
#include "terraincat.h"
#include <cstddef>
#include <cstdint>

namespace game {

struct TileVariationRecord
{
    LTerrainCategory terrain;
    LGroundCategory ground;
    int index;    /** < Read from NDX column. */
    int quantity; /** < Read from QTY column. */
    int hash;     /**< Computed as: index + ((ground.id + 16 * terrain.id) << 8) */
    TileVariationRecord* next;
};

static_assert(sizeof(TileVariationRecord) == 40,
              "Size of TileVariationRecord structure must be exactly 40 bytes");

static_assert(offsetof(TileVariationRecord, quantity) == 28,
              "TileVariationRecord::quantity offset must be 28 bytes");

struct CTileVariationData
{
    char unknown;
    char padding[3];
    std::uint32_t recordsTotal;
    TileVariationRecord** records;
    std::uint32_t recordsAllocated;
    std::uint32_t reallocationTreshold;
    int unknown3;
    char unknown4;
    char unknown5;
    char unknown6;
    char unknown7;
    char unknown8;
    char padding2[3];
    int unknown9;
    int unknown10;
    void* allocator;
};

static_assert(sizeof(CTileVariationData) == 44,
              "Size of CTileVariationData structure must be exactly 44 bytes");

static_assert(offsetof(CTileVariationData, reallocationTreshold) == 16,
              "CTileVariationData::reallocationTreshold offset must be 16 bytes");

/** Holds tile information read from GTileDBI.dbf. */
struct CTileVariation
{
    void* vftable;
    CTileVariationData* data;
};

static_assert(sizeof(CTileVariation) == 8,
              "Size of CTileVariation structure must be exactly 8 bytes");

} // namespace game

#endif // TILEVARIATION_H
