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
#include "tileindices.h"
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

assert_size(GroundTilesData, 56);

struct GroundTiles
{
    GroundTilesData* data;
};

assert_size(GroundTiles, 4);

/** Describes pair of tile border images for specific tile type. */
struct TileBorders
{
    TileArrayIndex tileArrayIndex; /**< Tile type of these borders. */
    std::uint32_t mainIndex;       /**< Main border image index in GrBorder.ff [1 - 15]. */
    int mainVariation;             /**< Random value in range [0 : 2]. */
    std::uint32_t additionalIndex; /**< Additional border image index in GrBorder.ff [17 - 31]. */
    int additionalVariation;       /**< Random value in range [0 : 2]. */
};

assert_size(TileBorders, 20);

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

assert_size(TileBordersInfo, 232);

struct IsoEngineGroundArrayElement
{
    SmartPtr<Vector<TerrainTilePtr>> tileVariants;
    bool water; /**< True if tile variants are belongs to water. */
    char padding[3];
};

assert_size(IsoEngineGroundArrayElement, 12);

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

assert_size(CIsoEngineGroundData, 5448);
assert_offset(CIsoEngineGroundData, tileBorders, 24);
assert_offset(CIsoEngineGroundData, isoGroundIndexer, 5412);
assert_offset(CIsoEngineGroundData, groundTextures, 5424);

/** Isometric engine responsible for ground rendering. */
struct CIsoEngineGround
{
    struct CGroundTexture;

    CIsoEngineGroundVftable* vftable;
    CIsoEngineGroundData* data;
};

assert_size(CIsoEngineGround, 8);

struct CIsoEngineGround::CGroundTexture : public IMqTexture
{
    CIsoEngineGround* isoEngineGround;
    CMqPoint textureOffset;
    bool dirty; /**< Indicates that texture has changes and should be redrawn. */
    char unknown5;
    bool hasWaterTiles; /**< Indicates that texture contains water tiles inside it. */
    bool hidden;        /**< Indicates that texture is fully hidden and should not be drawn. */
};

assert_size(CIsoEngineGround::CGroundTexture, 20);

struct IsoGroundTextureData
{
    TextureHandle textureHandle;
    CIsoEngineGround::CGroundTexture* texture;
    CMqPoint textureOffset;
};

assert_size(IsoGroundTextureData, 28);

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

assert_vftable_size(CIsoEngineGroundVftable, 2);

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

assert_size(TileBordersDrawInfo, 32);

namespace CGroundTextureApi {

IMqTextureVftable* vftable();

CIsoEngineGroundVftable* isoEngineVftable();

/** Temporary buffer for blend or alpha mask. */
void* borderMaskBuffer();

} // namespace CGroundTextureApi

} // namespace game

#endif // ISOENGINEGROUND_H
