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

#include "d2pair.h"
#include "d2set.h"
#include "texturehandle.h"
#include <cstdint>
#include <windows.h>

namespace game {

struct IMqTexture;
struct IDirectDrawSurface7;
struct IDirectDrawPalette;

struct RenderData22
{
    IMqTexture* texture;
    CMqPoint textureSize;
    std::uint32_t key;
    int type; /**< Assumption: hint. */
    bool unknown3;
    char padding;
    __int16 opacity;
};

static_assert(sizeof(RenderData22) == 24,
              "Size of RenderData22 structure must be exactly 24 bytes");

struct RenderData40
{
    TextureHandle textureHandle;
    RenderData22 data22;
};

static_assert(sizeof(RenderData40) == 40,
              "Size of RenderData40 structure must be exactly 40 bytes");

using RenderData40SetIntPair = Pair<Set<RenderData40>, int /* free index */>;

struct RenderData416_16
{
    int unknown;
    int unknown2;
    int unknown3;
    int unknown4;
};

static_assert(sizeof(RenderData416_16) == 16,
              "Size of RenderData416_16 structure must be exactly 16 bytes");

struct RenderData416
{
    int surfacesCount;
    IDirectDrawSurface7* surfaces[20];
    RenderData416_16 array2[20];
    bool unknown2;
    char padding[3];
    int unknown3;
    int type; /**< Assumption: hint. */
};

static_assert(sizeof(RenderData416) == 416,
              "Size of RenderData416 structure must be exactly 416 bytes");

struct RenderData420
{
    std::uint32_t key;
    RenderData416 data416;
};

static_assert(sizeof(RenderData420) == 420,
              "Size of RenderData420 structure must be exactly 420 bytes");

using PaletteEntryPair = Pair<PALETTEENTRY[256], IDirectDrawPalette*>;

static_assert(sizeof(PaletteEntryPair) == 1028,
              "Size of PaletteEntryPair structure must be exactly 1028 bytes");

struct RenderPaletteData
{
    std::uint32_t key;
    PaletteEntryPair pair;
};

} // namespace game

#endif // RENDERTYPES_H
