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

#include "d2assert.h"
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

assert_size(TileVariationRecord, 40);
assert_offset(TileVariationRecord, quantity, 28);

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

assert_size(CTileVariationData, 44);
assert_offset(CTileVariationData, reallocationTreshold, 16);

/** Holds tile information read from GTileDBI.dbf. */
struct CTileVariation
{
    void* vftable;
    CTileVariationData* data;
};

assert_size(CTileVariation, 8);

struct TileVariationIterator
{
    char unknown;
    char padding[3];
    TileVariationRecord** records;
    TileVariationRecord* record;
    std::uint32_t recordsTotal;
    bool found;
    char padding2[3];
};

assert_size(TileVariationIterator, 20);

namespace CTileVariationApi {

struct Api
{
    /** Checks correctness of all tile variation data records. */
    using CheckData = void(__stdcall*)(const CTileVariationData* data);
    CheckData checkData;

    /**
     * Checks correctness of specific tile variation data records.
     * @returns true if tile variation has no more than 'maxIndex' of records
     * with specified terrain and ground and each record has non zero quantity.
     */
    using CheckRecordsCorrect = bool(__stdcall*)(const CTileVariationData* data,
                                                 const LTerrainCategory* terrain,
                                                 const LGroundCategory* ground,
                                                 int maxIndex);
    CheckRecordsCorrect checkRecordsCorrect;
};

Api& get();

} // namespace CTileVariationApi

} // namespace game

#endif // TILEVARIATION_H
