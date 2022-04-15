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

#ifndef SURFACEDECOMPRESSDATA_H
#define SURFACEDECOMPRESSDATA_H

#include "d2color.h"
#include "mqrect.h"
#include <cstdint>
#include <windows.h>

namespace game {

/** 'BlitObject' in Verok's DisciplesGL. */
struct SurfaceDecompressData
{
    int noPalette;       /**< Value of zero means paletteEntries contain valid data. */
    void* surfaceMemory; /**< Initialized from DDSURFACEDESC2::lpSurface value. */
    CMqRect textureArea;
    /**
     * Initialized from DDSURFACEDESC2::lPitch value.
     * Specifies the number of bytes between the beginnings of two adjacent scan lines;
     * that is, the number of bytes to add to the beginning address of one scan line
     * to reach the beginning address of the next scan line below it.
     */
    int pitch;
    Color convertedColor;
    PALETTEENTRY paletteEntries[256];
    std::uint32_t rBitMask;
    std::uint32_t gBitMask;
    std::uint32_t bBitMask;
    std::uint32_t alphaBitMask;
};

static_assert(sizeof(SurfaceDecompressData) == 1072,
              "Size of SurfaceDecompressData structure must be exactly 1072 bytes");

} // namespace game

#endif // SURFACEDECOMPRESSDATA_H
