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

#ifndef ISOENGINEGROUND_H
#define ISOENGINEGROUND_H

#include "d2map.h"
#include "d2vector.h"
#include "mqpoint.h"
#include "mqtexture.h"
#include "smartptr.h"
#include "texturehandle.h"
#include <cstddef>
#include <cstdint>

namespace game {

struct CIsoEngineGroundVftable;
struct IMqRenderer2;
struct CMqRect;
struct IMqImage2;
struct CIsoStillBackground;
struct CMidIsoGroundIndexer;
struct CMqPresentationManager;
struct IsoGroundTextureData;
struct CBorderTile;
struct CTerrainTile;

using BorderTilePtr = SmartPtr<CBorderTile>;
using TerrainTilePtr = SmartPtr<CTerrainTile>;

/** Tile image size in pixels. */
constexpr int tileWidth{64};
constexpr int tileHeight{32};

constexpr int tileHalfWidth{tileWidth / 2};
constexpr int tileHalfHeight{tileHeight / 2};

/** Number of pixels in tile image. */
constexpr int tilePixelCount{tileWidth * tileHeight};

/** Used to convert between tile name prefixes and vice versa. */
enum class TilePrefix : std::uint32_t
{
    Water,   /**< WA */
    Neutral, /**< NE */
    Human,   /**< HU */
    Heretic, /**< HE */
    Dwarf,   /**< DW */
    Undead,  /**< UN */
    Black,   /**< BL */
    Elf,     /**< EL */
};

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

struct GroundTilesData
{
    /** Tracks how many tile images we have for each tile type. */
    Map<TilePrefix, std::uint32_t> tileTypes;
    /**
     * Tracks how many tile border images we have for each tile type.
     * Counts border images count for each of 32 (assumed) borders.
     */
    Map<TilePrefix, std::uint32_t[32]> tileBorders;
};

static_assert(sizeof(GroundTilesData) == 56,
              "Size of GroundTilesData structure must be exactly 56 bytes");

struct GroundTiles
{
    GroundTilesData* data;
};

static_assert(sizeof(GroundTiles) == 4, "Size of GroundTiles structure must be exactly 4 bytes");

/** Describes pair of tile border images for specific tile type. */
struct TileBorders
{
    TileArrayIndex tileArrayIndex; /**< Tile type of these borders. */
    std::uint32_t mainIndex;       /**< Main border image index in GrBorder.ff [1 - 15]. */
    int mainVariation;             /**< Random value in range [0 : 2]. */
    std::uint32_t additionalIndex; /**< Additional border image index in GrBorder.ff [17 - 31]. */
    int additionalVariation;       /**< Random value in range [0 : 2]. */
};

static_assert(sizeof(TileBorders) == 20, "Size of TileBorders structure must be exactly 20 bytes");

/** Holds data about all borders of a single tile. */
struct TileBordersInfo
{
    TileArrayIndex tileArrayIndex; /**< Tile type. Index for CIsoEngineGroundData::terrainTiles. */
    TileBorders borders[10];       /**< Borders of adjacent tiles that have different terrains. */
    int bordersTotal;              /**< Number of valid borders entries. */
    TileBorders waterBorders;      /**< Describes water borders. */
    bool hasWaterBorders;          /**< Indicates that water borders are present. */
    bool hidden;                   /**< Tile is patrially or fully hidden by fog of war. */
    char padding[2];
};

static_assert(sizeof(TileBordersInfo) == 232,
              "Size of TileBordersInfo structure must be exactly 232 bytes");

struct IsoEngineGroundArrayElement
{
    SmartPtr<Vector<TerrainTilePtr>> tileVariants;
    bool water; /**< True if tile variants are belongs to water. */
    char padding[3];
};

static_assert(sizeof(IsoEngineGroundArrayElement) == 12,
              "Size of IsoEngineGroundArrayElement structure must be exactly 12 bytes");

using IsoEngineGroundArray = Vector<IsoEngineGroundArrayElement>;

struct CIsoEngineGroundData
{
    CMqPoint isoViewSize; /**< Size of isometric map view in pixels. */
    IMqImage2* waterBackground;
    CIsoStillBackground* isoStillBackground;
    bool animatedIso;
    char padding[3];
    std::uint32_t mapSize; /**< Scenario map size in tiles. */
    /**
     * Borders information for each map tile.
     * Resized to mapSize * mapSize in CIsoEngineGround c-tor.
     */
    Vector<TileBordersInfo> tileBorders;
    TileBordersInfo outOfMapBorders; /**< Borders for tiles outside of map bounds. */
    bool dirty;
    char padding2[3];
    IsoEngineGroundArray terrainTiles; /**< Terrain tiles images in multiple variants. */
    /**
     * Border tile images in multiple variants.
     * Each vector contains less than 3 border tiles.
     * Array index computed as: borderIndex + 32 * TileArrayIndex.
     * Border index is in range [0 : 31].
     */
    Vector<BorderTilePtr> borderTiles[320];
    CMidIsoGroundIndexer* isoGroundIndexer; /**< Used for tile images variants selection. */
    SmartPtr<CMqPresentationManager> presentationMgr;
    Vector<IsoGroundTextureData> groundTextures;
    /** Number of textures along X axis (width) needed to fully cover ground area on screen. */
    std::uint32_t numTexturesAlongX;
    /** Number of textures along Y axis (height) needed to fully cover ground area on screen. */
    std::uint32_t numTexturesAlongY;
};

static_assert(sizeof(CIsoEngineGroundData) == 5448,
              "Size of CIsoEngineGroundData structure must be exactly 5448 bytes");

static_assert(offsetof(CIsoEngineGroundData, tileBorders) == 24,
              "CIsoEngineGroundData::tileBorders offset must be 24 bytes");

static_assert(offsetof(CIsoEngineGroundData, isoGroundIndexer) == 5412,
              "CIsoEngineGroundData::isoGroundIndexer offset must be 5412 bytes");

static_assert(offsetof(CIsoEngineGroundData, groundTextures) == 5424,
              "CIsoEngineGroundData::groundTextures offset must be 5424 bytes");

/** Isometric engine responsible for ground rendering. */
struct CIsoEngineGround
{
    struct CGroundTexture;

    CIsoEngineGroundVftable* vftable;
    CIsoEngineGroundData* data;
};

static_assert(sizeof(CIsoEngineGround) == 8,
              "Size of CIsoEngineGround structure must be exactly 8 bytes");

struct CIsoEngineGround::CGroundTexture : public IMqTexture
{
    CIsoEngineGround* isoEngineGround;
    CMqPoint textureOffset;
    bool dirty; /**< Indicates that texture has changes and should be redrawn. */
    char unknown5;
    bool hasWaterTiles; /**< Indicates that texture contains water tiles inside it. */
    bool hidden;        /**< Indicates that texture is fully hidden and should not be drawn. */
};

static_assert(sizeof(CIsoEngineGround::CGroundTexture) == 20,
              "Size of CIsoEngineGround::CGroundTexture structure must be exactly 20 bytes");

struct IsoGroundTextureData
{
    TextureHandle textureHandle;
    CIsoEngineGround::CGroundTexture* texture;
    CMqPoint textureOffset;
};

static_assert(sizeof(IsoGroundTextureData) == 28,
              "Size of IsoGroundTextureData structure must be exactly 28 bytes");

struct CIsoEngineGroundVftable
{
    using Destructor = void(__thiscall*)(CIsoEngineGround* thisptr, char flags);
    Destructor destructor;

    using Render = void(__thiscall*)(CIsoEngineGround* thisptr,
                                     IMqRenderer2* renderer,
                                     const CMqPoint* position,
                                     const CMqRect* area);
    Render render;
};

static_assert(sizeof(CIsoEngineGroundVftable) == 2 * sizeof(void*),
              "CIsoEngineGround vftable must have exactly 2 methods");

/** Holds information about tile borders rendering. */
struct TileBordersDrawInfo
{
    TileBordersInfo* tileInfo; /**< Tile borders to draw. */
    CMqPoint srcPos;           /**< Source position for CTerrainTile::Draw. */
    CMqPoint dstPos;           /**< Destination position for CTerrainTile::Draw. */
    CMqPoint pos3;
    bool hidden; /**< Tile hidden in fog of war. */
    char padding[3];
};

static_assert(sizeof(TileBordersDrawInfo) == 32,
              "Size of TileBordersDrawInfo structure must be exactly 32 bytes");

namespace CGroundTextureApi {

IMqTextureVftable* vftable();

CIsoEngineGroundVftable* isoEngineVftable();

/** Temporary buffer for blend or alpha mask. */
void* borderMaskBuffer();

} // namespace CGroundTextureApi

} // namespace game

#endif // ISOENGINEGROUND_H
