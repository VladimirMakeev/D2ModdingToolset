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

#ifndef RENDERTYPES_H
#define RENDERTYPES_H

#include "d2assert.h"
#include "d2list.h"
#include "d2pair.h"
#include "d2set.h"
#include "mqrect.h"
#include "texturehandle.h"
#include <Windows.h>
#include <cstdint>

namespace game {

struct IMqTexture;
struct IDirectDrawSurface7;
struct IDirectDrawPalette;

struct RenderData22
{
    IMqTexture* texture;
    CMqPoint textureSize;
    std::uint32_t paletteKey; /**< Used for CRendererImpl::paletteEntryMap search.
                               * Value of zero reserved for 'default' palette.
                               */
    std::uint32_t hint;       /**< Can be 0, 1 or 2.
                               * Passed through IMqTexturer2::addTexture, for example.
                               * Used in IMqRasterizer::createOffscreenSurface.
                               */
    bool hasCustomPalette;    /**< paletteKey is not zero, valid palette should be passed to
                               * IMqRasterizer::createSurfaceDecomp.
                               */
    char padding;
    std::int16_t opacity;
};

assert_size(RenderData22, 24);

/** Maps texture handles to IMqTextures. */
struct RenderData40
{
    TextureHandle textureHandle;
    RenderData22 data22;
};

assert_size(RenderData40, 40);

using RenderData40SetIntPair = Pair<Set<RenderData40>,
                                    std::uint32_t /* free texture handle index */>;

/** Name chosen according to log message in 0x519000. */
struct TextureSurface
{
    int surfacesCount;
    IDirectDrawSurface7* surfaces[20]; /**< Used in 0x519000 for alpha blend. */
    CMqRect areas[20];
    bool unknown2;
    char padding[3];
    int batchNumber;    /**< Can be set to -1. See IMqTexturer2::Method6 or 0x51b3b0 calls. */
    std::uint32_t hint; /**< Same as RenderData22::hint.
                         * Used in IMqRasterizer::createOffscreenSurface.
                         */
};

assert_size(TextureSurface, 416);

using PaletteEntryPair = Pair<::tagPALETTEENTRY[256], IDirectDrawPalette*>;

assert_size(PaletteEntryPair, 1028);

struct RenderOperation
{
    /** Operation type. */
    enum class Type : int
    {
        Draw,         /**< Calls IMqRasterizer::alphaBlend. */
        StartScaling, /**< Calls IMqRasterizer::startScaling. */
        EndScaling,   /**< Calls IMqRasterizer::endScaling. */
    };

    Type type;
    TextureHandle textureHandle;
    CMqPoint offset;
    CMqPoint start;
    CMqPoint size;
    CMqRect area;
    /**
     * Holds opacity or negative value.
     * In case of negative value, RenderData22::opacity is used for alpha blend.
     */
    int maybeOpacity;
};

assert_size(RenderOperation, 64);
assert_offset(RenderOperation, offset, 20);

/** Stores rendering operations that later will be packed into a batch. */
struct RenderQueue
{
    RenderOperation* operations;
    int operationsCount; /**< Current number of operations in queue. */
    int maxOperations;   /**< Set to 3072 in c-tor. */
    CMqRect windowClientArea;
    List<CMqRect> rectList;
    List<CMqRect> rectList2;
    int unknown;
};

assert_size(RenderQueue, 64);

struct RenderBatchElement
{
    TextureHandle textureHandle;
    /**
     * True if there is a TextureSurface found in CRendererImpl::textureSurfaceMap.
     * It means there is no need to create texture surface for this batch operation.
     */
    bool textureSurfaceExist;
    /**
     * True if:
     * - one of the TextureSurface::surfaces was lost and restored
     * - TextureSurface was created for this batch
     * - RenderData22 found by textureHandle returned texture->isDirty().
     */
    bool dirtyOrRestored;
    char padding[2];
};

assert_size(RenderBatchElement, 20);

/**
 * Stores batch elements to render.
 * Populated from RenderQueue using RenderOperations with type Draw.
 */
struct RenderBatch
{
    RenderBatchElement* array;
    int elementCount;
    int maxElements;
};

assert_size(RenderBatch, 12);

} // namespace game

#endif // RENDERTYPES_H
