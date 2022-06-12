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

#include "isoenginegroundhooks.h"
#include "2dengine.h"
#include "bordertile.h"
#include "isostillbackground.h"
#include "midisogroundindexer.h"
#include "mqimage2.h"
#include "mqrenderer2.h"
#include "surfacedecompressdata.h"
#include "terraintile.h"
#include "utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <fmt/format.h>
#include <vector>

namespace hooks {

static constexpr game::Color black{0};
static constexpr game::Color magenta{0xff00ffu};
static constexpr game::Color white{0xffffffffu};

/** Array with info about what (and how) needs to be drawn inside of each CGroundTexture. */
static std::vector<game::TileBordersDrawInfo> drawInfoArray;

void updateTileBordersDrawInfo(game::CIsoEngineGround::CGroundTexture* groundTexture);

static std::uint32_t getTileVariantsCount(const game::IsoEngineGroundArrayElement& element)
{
    auto variants{element.tileVariants.data};
    if (!variants) {
        return 0;
    }

    return variants->end - variants->bgn;
}

static game::CTerrainTile* groundArrayElementGetTile(game::IsoEngineGroundArrayElement& element,
                                                     std::uint32_t tileVariant)
{
    const auto variantsTotal{getTileVariantsCount(element)};
    if (!variantsTotal) {
        return nullptr;
    }

    return element.tileVariants.data->bgn[tileVariant % variantsTotal].data;
}

static game::CTerrainTile* getFirstTerrainTile(game::IsoEngineGroundArray& groundArray)
{
    for (auto element = groundArray.bgn; element != groundArray.end; ++element) {
        if (element->water) {
            continue;
        }

        const auto variantsTotal{getTileVariantsCount(*element)};

        for (std::uint32_t i = 0; i < variantsTotal; ++i) {
            auto tile{groundArrayElementGetTile(*element, i)};
            if (tile) {
                return tile;
            }
        }
    }

    return nullptr;
}

static game::CTerrainTile* getTerrainTile(game::CIsoEngineGround* engineGround,
                                          const game::CMqPoint* position,
                                          game::TileArrayIndex tileArrayIndex)
{
    const auto idx{static_cast<int>(tileArrayIndex)};

    auto& element{engineGround->data->terrainTiles.bgn[idx]};
    if (element.water) {
        return nullptr;
    }

    auto groundIndexer{engineGround->data->isoGroundIndexer};
    auto variant{groundIndexer->vftable->getData(groundIndexer, position)};

    return groundArrayElementGetTile(element, variant);
}

static std::uint32_t getBorderTileIndex(std::uint32_t borderIndex,
                                        game::TileArrayIndex tileArrayIndex)
{
    return borderIndex + 32 * static_cast<int>(tileArrayIndex);
}

static game::CBorderTile* getBorderTile(game::Vector<game::BorderTilePtr>* borderTiles,
                                        std::uint32_t borderIndex,
                                        int borderVariation,
                                        game::TileArrayIndex tileArrayIndex)
{
    if (borderIndex && borderIndex != 16) {
        const auto idx{getBorderTileIndex(borderIndex, tileArrayIndex)};
        assert(idx < 320);

        auto begin{borderTiles[idx].bgn};
        auto end{borderTiles[idx].end};

        auto total{end - begin};
        assert(total > 0);

        return begin[borderVariation % total].data;
    }

    return nullptr;
}

static game::CBorderTile* getMainBorderTile(game::Vector<game::BorderTilePtr>* borderTiles,
                                            const game::TileBorders& borders,
                                            game::TileArrayIndex tileArrayIndex)
{
    return getBorderTile(borderTiles, borders.mainIndex, borders.mainVariation, tileArrayIndex);
}

static game::CBorderTile* getAdditionalBorderTile(game::Vector<game::BorderTilePtr>* borderTiles,
                                                  const game::TileBorders& borders,
                                                  game::TileArrayIndex tileArrayIndex)
{
    return getBorderTile(borderTiles, borders.additionalIndex, borders.additionalVariation,
                         tileArrayIndex);
}

static void drawTerrainTiles(game::CIsoEngineGround* engineGround,
                             game::SurfaceDecompressData* surfaceData,
                             game::TileBordersDrawInfo& drawInfo,
                             const std::uint32_t* blendMask,
                             const std::uint32_t* alphaMask,
                             game::TileArrayIndex tileArrayIndex)
{
    auto& position{drawInfo.pos3};
    auto srcPos{&drawInfo.srcPos};
    auto dstPos{&drawInfo.dstPos};

    using namespace game;

    auto terrainTile2{getTerrainTile(engineGround, &position, tileArrayIndex)};
    if (terrainTile2) {
        terrainTile2->vftable->draw(terrainTile2, surfaceData, srcPos, dstPos,
                                    &surfaceData->textureArea, blendMask, alphaMask);
    }

    if (srcPos->y + tileHeight > 192) {
        const CMqPoint pos{position.x, position.y + tileHeight};

        auto terrainTile3{getTerrainTile(engineGround, &pos, tileArrayIndex)};
        if (terrainTile3) {
            CMqPoint src{srcPos->x, srcPos->y - 192};

            terrainTile3->vftable->draw(terrainTile3, surfaceData, &src, dstPos,
                                        &surfaceData->textureArea, blendMask, alphaMask);
        }
    }

    if (srcPos->x + tileWidth > 192) {
        const CMqPoint pos{position.x + tileWidth, position.y};

        auto terrainTile4{getTerrainTile(engineGround, &pos, tileArrayIndex)};
        if (terrainTile4) {
            CMqPoint src{srcPos->x - 192, srcPos->y};

            terrainTile4->vftable->draw(terrainTile4, surfaceData, &src, dstPos,
                                        &surfaceData->textureArea, blendMask, alphaMask);
        }
    }

    if (srcPos->x + tileWidth > 192 && srcPos->y + tileHeight > 192) {
        const CMqPoint pos{position.x + tileHalfWidth, position.y + tileHalfHeight};

        auto terrainTile5{getTerrainTile(engineGround, &pos, tileArrayIndex)};
        if (terrainTile5) {
            CMqPoint src{srcPos->x - 192, srcPos->y - 192};

            terrainTile5->vftable->draw(terrainTile5, surfaceData, &src, dstPos,
                                        &surfaceData->textureArea, blendMask, alphaMask);
        }
    }
}

static void getMasks(game::SurfaceDecompressData* surfaceData,
                     game::CBorderTile* mainBorder,
                     game::CBorderTile* additionalBorder,
                     const std::uint32_t** blendMask,
                     const std::uint32_t** alphaMask)
{
    const auto& surfaceApi{game::SurfaceDecompressDataApi::get()};

    if (mainBorder && additionalBorder) {
        auto maskBuffer{game::CGroundTextureApi::borderMaskBuffer()};

        if (mainBorder->vftable->is8BppImage(mainBorder)) {
            auto buffer = reinterpret_cast<std::uint8_t*>(maskBuffer);

            auto additionalBorderPixel{additionalBorder->vftable->getByteData(additionalBorder)};
            auto mainBorderPixel{mainBorder->vftable->getByteData(mainBorder)};

            int pixelCount{game::tilePixelCount};
            do {
                std::uint8_t pixel = *mainBorderPixel;
                if (!pixel) {
                    pixel = *additionalBorderPixel;
                }

                *buffer++ = pixel;

                ++mainBorderPixel;
                ++additionalBorderPixel;
                --pixelCount;
            } while (pixelCount);

            *blendMask = reinterpret_cast<const std::uint32_t*>(maskBuffer);
            *alphaMask = nullptr;
        } else {
            auto buffer = reinterpret_cast<std::uint16_t*>(maskBuffer);

            auto additionalBorderPixel{additionalBorder->vftable->getWordData(additionalBorder)};
            auto mainBorderPixel{mainBorder->vftable->getWordData(mainBorder)};

            const auto magentaConverted{surfaceApi.convertColor(surfaceData, &magenta)};

            int pixelCount{game::tilePixelCount};
            do {
                std::uint16_t pixel = *mainBorderPixel;
                if (pixel == magentaConverted) {
                    pixel = *additionalBorderPixel;
                }

                *buffer++ = pixel;

                ++mainBorderPixel;
                ++additionalBorderPixel;
                --pixelCount;
            } while (pixelCount);

            *blendMask = nullptr;
            *alphaMask = reinterpret_cast<const std::uint32_t*>(maskBuffer);
        }

        return;
    }

    if (mainBorder) {
        if (mainBorder->vftable->is8BppImage(mainBorder)) {
            *blendMask = (const std::uint32_t*)mainBorder->vftable->getByteData(mainBorder);
            *alphaMask = nullptr;
        } else {
            *blendMask = nullptr;
            *alphaMask = (const std::uint32_t*)mainBorder->vftable->getWordData(mainBorder);
        }

        return;
    }

    if (additionalBorder) {
        if (additionalBorder->vftable->is8BppImage(additionalBorder)) {
            *blendMask = (const std::uint32_t*)additionalBorder->vftable->getByteData(
                additionalBorder);
            *alphaMask = nullptr;
        } else {
            *blendMask = nullptr;
            *alphaMask = (const std::uint32_t*)additionalBorder->vftable->getWordData(
                additionalBorder);
        }
    }
}

void __stdcall groundTextureDrawHooked(game::CIsoEngineGround::CGroundTexture* thisptr,
                                       game::SurfaceDecompressData* surfaceData)
{
    using namespace game;

    auto engineGround{thisptr->isoEngineGround};
    auto& borderTiles{engineGround->data->borderTiles};

    auto groundIndexer{engineGround->data->isoGroundIndexer};
    groundIndexer->vftable->updateData(groundIndexer);

    updateTileBordersDrawInfo(thisptr);

    if (thisptr->hidden) {
        return;
    }

    const auto& surfaceApi{SurfaceDecompressDataApi::get()};

    if (thisptr->hasWaterTiles) {
        if (engineGround->data->animatedIso) {
            // Fill entire area with transparent (magenta) color and remember it
            const auto magentaConverted{surfaceApi.convertColor(surfaceData, &magenta)};

            surfaceApi.fillArea(surfaceData, magentaConverted, &surfaceData->textureArea);
            surfaceApi.setColor(surfaceData, magentaConverted);
        } else {
            auto stillBackground{engineGround->data->isoStillBackground};
            if (stillBackground) {
                // Draw still background if we have it
                stillBackground->vftable->draw(stillBackground, surfaceData);
            } else {
                // Otherwise fill area with black color
                const auto blackConverted{surfaceApi.convertColor(surfaceData, &black)};
                surfaceApi.fillArea(surfaceData, blackConverted, &surfaceData->textureArea);
            }
        }
    }

    if (!engineGround->data->animatedIso) {
        surfaceApi.setColor(surfaceData, white);
    }

    auto& terrainTiles{engineGround->data->terrainTiles};

    auto terrainTile{getFirstTerrainTile(terrainTiles)};
    assert(terrainTile != nullptr);

    for (auto& drawInfo : drawInfoArray) {
        const std::uint32_t* blendMask{};
        const std::uint32_t* alphaMask{};

        if (drawInfo.hidden) {
            // Draw under the fog of war?
            terrainTile->vftable->method2(terrainTile, surfaceData, &drawInfo.dstPos,
                                          &surfaceData->textureArea);
            continue;
        }

        auto tileInfo{drawInfo.tileInfo};
        if (tileInfo->bordersTotal < 0) {
            // No borders, nothing to do here
            continue;
        }

        const auto tileType{tileInfo->tileArrayIndex};

        if (!terrainTiles.bgn[static_cast<int>(tileType)].water) {
            // Ground tile, draw terrain tiles
            drawTerrainTiles(engineGround, surfaceData, drawInfo, blendMask, alphaMask, tileType);
        } else {
            // Water tile, check for coast borders and draw if present
            if (tileInfo->hasWaterBorders) {
                const auto& waterBorders{tileInfo->waterBorders};

                auto main{getMainBorderTile(borderTiles, waterBorders, tileType)};
                if (main) {
                    main->vftable->draw(main, surfaceData, &drawInfo.dstPos,
                                        &surfaceData->textureArea);
                }

                auto additional{getAdditionalBorderTile(borderTiles, waterBorders, tileType)};
                if (additional) {
                    additional->vftable->draw(additional, surfaceData, &drawInfo.dstPos,
                                              &surfaceData->textureArea);
                }
            }
        }

        // Draw border tiles
        for (int border = 0; border < tileInfo->bordersTotal; ++border) {
            auto& borders{tileInfo->borders[border]};
            const auto bordersType{borders.tileArrayIndex};

            auto mainBorder{getMainBorderTile(borderTiles, borders, bordersType)};
            auto additionalBorder{getAdditionalBorderTile(borderTiles, borders, bordersType)};

            getMasks(surfaceData, mainBorder, additionalBorder, &blendMask, &alphaMask);

            drawTerrainTiles(engineGround, surfaceData, drawInfo, blendMask, alphaMask,
                             bordersType);
        }
    }

    thisptr->dirty = false;
    return;
}

static std::uint32_t getTileMainBorder(int positionX,
                                       int positionY,
                                       int mapSize,
                                       game::TileBordersInfo* bordersInfo,
                                       game::TileArrayIndex tileType,
                                       const game::TileArrayIndex* expectedAdjacent = nullptr)
{
    using namespace game;

    // clang-format off
    static const std::array<CMqPoint, 4> offsets = {{
        CMqPoint{-1,  0},
        CMqPoint{ 0, -1},
        CMqPoint{ 1,  0},
        CMqPoint{ 0,  1}
    }};
    // clang-format on

    // Assume there are no borders by default
    std::uint32_t mainBorder{};
    for (std::uint32_t i = 0; i < offsets.size(); ++i) {
        const int x = positionX + offsets[i].x;
        const int y = positionY + offsets[i].y;

        if (x < 0 || y < 0 || x >= mapSize || y >= mapSize) {
            // Skip tiles outside of the map.
            // With assumption above it means
            // we do not draw tile borders on map edges.
            continue;
        }

        auto& adjacentTile{bordersInfo[x + y * mapSize]};
        if (adjacentTile.tileArrayIndex == tileType) {
            continue;
        }

        if (expectedAdjacent && adjacentTile.tileArrayIndex != *expectedAdjacent) {
            continue;
        }

        mainBorder |= 1 << i;
    }

    return mainBorder;
}

static std::uint32_t getTileAdditionalBorder(std::uint32_t mainBorder,
                                             int positionX,
                                             int positionY,
                                             int mapSize,
                                             game::TileBordersInfo* bordersInfo,
                                             game::TileArrayIndex tileType,
                                             const game::TileArrayIndex* expectedAdjacent = nullptr)
{
    using namespace game;

    // Assume there are no additional borders by default
    std::uint32_t additionalBorder{};

    // Check 4 triplets of adjacent tiles
    for (std::uint32_t triplet = 0; triplet < 4; ++triplet) {
        // Skip triplet check if main border index contains set bit at triplet position.
        // We don't want to draw additional borders above main.
        if (mainBorder & (1 << triplet)) {
            continue;
        }

        // clang-format off
        static const std::array<std::array<CMqPoint, 3>, 4> triplets = {{
            {{ CMqPoint{-1,  0}, CMqPoint{-1, -1}, CMqPoint{ 0, -1} }},
            {{ CMqPoint{ 0, -1}, CMqPoint{ 1, -1}, CMqPoint{ 1,  0} }},
            {{ CMqPoint{ 1,  0}, CMqPoint{ 1,  1}, CMqPoint{ 0,  1} }},
            {{ CMqPoint{ 0,  1}, CMqPoint{-1,  1}, CMqPoint{-1,  0} }}
        }};
        // clang-format on

        const auto& offsets{triplets[triplet]};

        std::uint32_t tmpBorder{};
        for (std::uint32_t i = 0; i < offsets.size(); ++i) {
            const int x = positionX + offsets[i].x;
            const int y = positionY + offsets[i].y;

            if (x < 0 || y < 0 || x >= mapSize || y >= mapSize) {
                // Skip tiles outside of the map.
                // With additionalBorder being zero by default it means
                // we do not draw additional tile borders on map edges.
                continue;
            }

            auto& adjacentTile{bordersInfo[x + y * mapSize]};
            if (adjacentTile.tileArrayIndex == tileType) {
                continue;
            }

            if (expectedAdjacent && adjacentTile.tileArrayIndex != *expectedAdjacent) {
                continue;
            }

            tmpBorder |= 1 << i;
        }

        // Middle tile from triplet had ground, it is an additional border
        additionalBorder |= static_cast<std::uint32_t>(tmpBorder == 2) << triplet;
    }

    if (additionalBorder) {
        // Adjust border index to 17 - 31
        // 16 is an invalid border index just as 0
        additionalBorder += 16;
    }

    return additionalBorder;
}

static void updateTileBorders(game::CIsoEngineGround* thisptr)
{
    using namespace game;

    if (!thisptr->data->dirty) {
        return;
    }

    thisptr->data->dirty = false;

    auto& tileBorders{thisptr->data->tileBorders};
    const auto mapSize{static_cast<int>(thisptr->data->mapSize)};

    for (int indexY = 0; indexY < mapSize; ++indexY) {
        for (int indexX = 0; indexX < mapSize; ++indexX) {
            auto& tile{tileBorders.bgn[indexX + indexY * mapSize]};

            const auto tileType{tile.tileArrayIndex};

            tile.hasWaterBorders = false;
            tile.bordersTotal = 0;

            // +1 to get borders that can be drawn above current tile, not below
            for (int i = static_cast<int>(tileType) + 1; i < 8; ++i) {
                const auto expected = static_cast<TileArrayIndex>(i);

                const auto mainIndex{getTileMainBorder(indexX, indexY, mapSize, tileBorders.bgn,
                                                       tileType, &expected)};
                const auto additionalIndex{getTileAdditionalBorder(mainIndex, indexX, indexY,
                                                                   mapSize, tileBorders.bgn,
                                                                   tileType, &expected)};

                if (mainIndex || additionalIndex) {
                    auto& border = tile.borders[tile.bordersTotal];
                    border.tileArrayIndex = expected;
                    border.mainIndex = mainIndex;
                    border.additionalIndex = additionalIndex;

                    ++tile.bordersTotal;
                }
            }

            if (tile.bordersTotal) {
                // Sort in such a way that borders with lesser types are (drawn) first
                std::sort(&tile.borders[0], &tile.borders[tile.bordersTotal],
                          [](const TileBorders& a, const TileBorders& b) {
                              return a.tileArrayIndex < b.tileArrayIndex;
                          });

                // Water tiles that have borders with ground (coastal tiles)
                // should also have water borders
                if (tileType == TileArrayIndex::Water) {
                    const auto mainIndex{getTileMainBorder(indexX, indexY, mapSize, tileBorders.bgn,
                                                           TileArrayIndex::Water)};
                    const auto additionalIndex{getTileAdditionalBorder(mainIndex, indexX, indexY,
                                                                       mapSize, tileBorders.bgn,
                                                                       TileArrayIndex::Water)};

                    if (mainIndex || additionalIndex) {
                        auto& waterBorders{tile.waterBorders};

                        waterBorders.tileArrayIndex = TileArrayIndex::Water;
                        waterBorders.mainIndex = mainIndex;
                        waterBorders.additionalIndex = additionalIndex;

                        tile.hasWaterBorders = true;
                    }
                }
            }
        }
    }
}

/** Projects tile coordinate to the screen coordinate in isometric view. */
static game::CMqPoint tilePositionToScreen(const game::CMqPoint& p)
{
    using namespace game;

    return CMqPoint{p.x * tileHalfWidth - p.y * tileHalfWidth,
                    p.x * tileHalfHeight + p.y * tileHalfHeight};
}

static game::CMqPoint textureCoordToTileCoord(const game::CMqPoint& textureCoordinate)
{
    using namespace game;

    // clang-format off
    static const std::array<char, tilePixelCount> tileIndices = {{
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,
        1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,
        1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,
        3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,
        3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,
        3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
    }};
    // clang-format on

    const auto x{textureCoordinate.x};
    const auto y{textureCoordinate.y};

    const int adjX{x <= 0 ? x - tileWidth + 1 : x};
    const int adjY{y <= 0 ? y - tileHeight + 1 : y};

    const int tileIndexX{adjX / tileWidth};
    const int tileIndexY{adjY / tileHeight};

    int coordX{tileIndexX + tileIndexY};
    int coordY{tileIndexY - tileIndexX};

    const auto index{65 * std::abs(y - tileHeight * tileIndexY)
                     + std::abs(x - tileIndexX * tileWidth)};
    const int tileIndex{tileIndices[index]};

    switch (tileIndex) {
    case 0:
    default:
        break;
    case 1:
        --coordX;
        break;
    case 2:
        --coordY;
        break;
    case 3:
        ++coordY;
        break;
    case 4:
        ++coordX;
        break;
    }

    return game::CMqPoint{coordX, coordY};
}

void __fastcall isoEngineGroundRenderHooked(game::CIsoEngineGround* thisptr,
                                            int /*%edx*/,
                                            game::IMqRenderer2* renderer,
                                            const game::CMqPoint* position,
                                            const game::CMqRect* area)
{
    using namespace game;

    auto& groundTextures{thisptr->data->groundTextures};

    for (auto textureData = groundTextures.bgn; textureData != groundTextures.end; ++textureData) {
        auto groundTexture{textureData->texture};

        if (groundTexture->vftable->isDirty(groundTexture)) {
            updateTileBordersDrawInfo(groundTexture);
        }

        if (groundTexture->hidden) {
            continue;
        }

        const auto& textureOffset{textureData->textureOffset};

        CMqPoint start{textureOffset.x + position->x, textureOffset.y + position->y};
        CMqPoint offset{};
        CMqPoint* size{&textureData->textureHandle.textureSize};

        if (thisptr->data->animatedIso && groundTexture->hasWaterTiles) {
            auto waterBgnd{thisptr->data->waterBackground};
            if (waterBgnd) {
                waterBgnd->vftable->render(waterBgnd, renderer, &start, &offset, size, area);
            }
        }

        renderer->vftable->drawTexture(renderer, &textureData->textureHandle, &start, &offset, size,
                                       area);
    }
}

static void splitTextureOffset(const game::CMqPoint& textureOffset,
                               game::CMqPoint& tileCoordinate,
                               game::CMqPoint& dst)
{
    tileCoordinate = textureCoordToTileCoord(textureOffset);

    const auto screenPos{tilePositionToScreen(tileCoordinate)};

    dst = screenPos - textureOffset;
}

static game::TileBordersInfo* getTileBordersInfo(game::CIsoEngineGround& engineGround,
                                                 const game::CMqPoint& mapPosition)
{
    const auto mapSize{static_cast<int>(engineGround.data->mapSize)};
    const auto x{mapPosition.x};
    const auto y{mapPosition.y};

    if (x >= 0 && x < mapSize && y >= 0 && y < mapSize) {
        return &engineGround.data->tileBorders.bgn[x + y * mapSize];
    }

    return &engineGround.data->outOfMapBorders;
}

/** Returns true if specified map position is completely hidden by fog of war. */
static bool isPositionFullyHidden(game::CIsoEngineGround& engineGround,
                                  const game::CMqPoint& mapPosition)
{
    using namespace game;

    if (!getTileBordersInfo(engineGround, mapPosition)->hidden) {
        return false;
    }

    // clang-format off
    static const std::array<CMqPoint, 8> offsets = {{
        CMqPoint{-1,  1},
        CMqPoint{ 0,  1},
        CMqPoint{ 1,  1},
        CMqPoint{ 1,  0},
        CMqPoint{ 1, -1},
        CMqPoint{ 0, -1},
        CMqPoint{-1, -1},
        CMqPoint{-1,  0}
    }};
    // clang-format on

    const auto mapSize{static_cast<int>(engineGround.data->mapSize)};

    // Check if adjacent tiles are hidden too
    for (const auto& offset : offsets) {
        const CMqPoint position{mapPosition.x + offset.x, mapPosition.y + offset.y};

        if (position.x >= 0 && position.x < mapSize && position.y >= 0 && position.y < mapSize) {
            if (!getTileBordersInfo(engineGround, position)->hidden) {
                return false;
            }
        }
    }

    return true;
}

static void fillBordersDrawInfo(game::CIsoEngineGround::CGroundTexture* groundTexture,
                                const game::CMqPoint& mapPosition,
                                const game::CMqPoint& dstPos,
                                game::TileBordersDrawInfo& drawInfo)
{
    using namespace game;

    auto& engineGround{*groundTexture->isoEngineGround};

    auto tileBorders{getTileBordersInfo(engineGround, mapPosition)};
    drawInfo.tileInfo = tileBorders;
    drawInfo.dstPos = dstPos;

    if (isPositionFullyHidden(engineGround, mapPosition)) {
        drawInfo.hidden = true;
        return;
    }

    const auto mapSize{engineGround.data->mapSize};

    drawInfo.srcPos = groundTexture->textureOffset;
    drawInfo.srcPos += dstPos;
    drawInfo.srcPos.x += mapSize * tileWidth;
    drawInfo.srcPos.y += mapSize * tileHeight;

    drawInfo.pos3 = drawInfo.srcPos;

    drawInfo.srcPos.x %= 192;
    drawInfo.srcPos.y %= 192;

    if (engineGround.data->terrainTiles.bgn[static_cast<int>(tileBorders->tileArrayIndex)].water) {
        groundTexture->hasWaterTiles = true;
    }

    drawInfo.hidden = false;
    groundTexture->hidden = false;
}

void updateTileBordersDrawInfo(game::CIsoEngineGround::CGroundTexture* groundTexture)
{
    using namespace game;

    auto engineGround{groundTexture->isoEngineGround};

    updateTileBorders(engineGround);

    drawInfoArray.clear();

    groundTexture->hasWaterTiles = false;
    groundTexture->hidden = true;

    CMqPoint tileCoordinate{};
    CMqPoint dst{};
    splitTextureOffset(groundTexture->textureOffset, tileCoordinate, dst);
    --tileCoordinate.x;

    for (int y = dst.y - tileHalfHeight; y < 128;
         y += tileHeight, ++tileCoordinate.x, ++tileCoordinate.y) {
        CMqPoint mapPos{tileCoordinate};

        for (int x = dst.x - tileHalfWidth; x < 128; x += tileWidth, --mapPos.y) {
            const CMqPoint dstPos{x, y};
            TileBordersDrawInfo drawInfo{};

            fillBordersDrawInfo(groundTexture, mapPos, dstPos, drawInfo);
            drawInfoArray.push_back(drawInfo);

            ++mapPos.x;

            const CMqPoint dstPos2{x + tileHalfWidth, y + tileHalfHeight};
            TileBordersDrawInfo drawInfo2{};

            fillBordersDrawInfo(groundTexture, mapPos, dstPos2, drawInfo2);
            drawInfoArray.push_back(drawInfo2);
        }
    }

    if (groundTexture->hidden) {
        groundTexture->dirty = false;
    }
}

} // namespace hooks
