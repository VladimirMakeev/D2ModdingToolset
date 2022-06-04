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

assert_size(RenderData22, 24);

struct RenderData40
{
    TextureHandle textureHandle;
    RenderData22 data22;
};

assert_size(RenderData40, 40);

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

assert_size(RenderData416, 416);

struct RenderData420
{
    std::uint32_t key;
    RenderData416 data416;
};

assert_size(RenderData420, 420);

using PaletteEntryPair = Pair<PALETTEENTRY[256], IDirectDrawPalette*>;

assert_size(PaletteEntryPair, 1028);

struct RenderPaletteData
{
    std::uint32_t key;
    PaletteEntryPair pair;
};

} // namespace game

#endif // RENDERTYPES_H
