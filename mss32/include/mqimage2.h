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

#ifndef MQIMAGE2_H
#define MQIMAGE2_H

#include "d2assert.h"
#include <cstdint>

namespace game {

struct IMqImage2Vftable;
struct CMqPoint;
struct CMqRect;
struct IMqRenderer2;

/** Assumption: base class for all images. */
struct IMqImage2
{
    IMqImage2Vftable* vftable;
};

struct IMqImage2Vftable
{
    using Destructor = void(__thiscall*)(IMqImage2* thisptr, char flags);
    Destructor destructor;

    /** Returns image dimensions as CMqPoint(width, height). */
    using GetSize = CMqPoint*(__thiscall*)(const IMqImage2* thisptr, CMqPoint* size);
    GetSize getSize;

    /** Sets current image index. */
    using SetImageIndex = std::uint32_t(__thiscall*)(IMqImage2* thisptr, std::uint32_t imageIndex);
    SetImageIndex setImageIndex;

    /** Returns current image index. */
    using GetImageIndex = std::uint32_t(__thiscall*)(const IMqImage2* thisptr);
    GetImageIndex getImageIndex;

    /**
     * Returns number of images depending on context.
     * For animation related derived types, returns number of frames.
     * For multi-layered images returns number of layers.
     * For common images returns 1.
     */
    using GetImagesCount = std::uint32_t(__thiscall*)(const IMqImage2* thisptr);
    GetImagesCount getImagesCount;

    /** Assumtion: renders image in CMqRect(start + offset, size) area. */
    using Render = void(__thiscall*)(const IMqImage2* thisptr,
                                     IMqRenderer2* renderer,
                                     const CMqPoint* start,
                                     const CMqPoint* offset,
                                     const CMqPoint* size,
                                     const CMqRect* area);
    Render render;

    using SetUnknown = void(__thiscall*)(IMqImage2* thisptr);
    SetUnknown setUnknown;

    using ResetUnknown = void(__thiscall*)(IMqImage2* thisptr);
    ResetUnknown resetUnknown;

    using Method8 = int(__thiscall*)(IMqImage2* thisptr);
    Method8 method8;
};

assert_vftable_size(IMqImage2Vftable, 9);

} // namespace game

#endif // MQIMAGE2_H
