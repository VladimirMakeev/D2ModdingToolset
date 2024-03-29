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

#ifndef MQDISPLAY2_H
#define MQDISPLAY2_H

#include "d2assert.h"

namespace game {

struct IMqDisplay2Vftable;
struct CMqPoint;
struct DisplaySettings;
struct CMqRect;

template <typename T>
struct IMqDisplay2T
{
    T* vftable;
};

struct IMqDisplay2 : public IMqDisplay2T<IMqDisplay2Vftable>
{ };

// Virtual table does not contain destructor
struct IMqDisplay2Vftable
{
    /** Returns current display settings. */
    using GetDisplaySettings = DisplaySettings*(__thiscall*)(IMqDisplay2* thisptr,
                                                             DisplaySettings* settings);
    GetDisplaySettings getDisplaySettings;

    using Method1 = void*(__thiscall*)(IMqDisplay2* thisptr, void* a2);
    Method1 method1;

    /** Returns size of screen surface. */
    using GetScreenSurfaceSize = CMqPoint*(__thiscall*)(IMqDisplay2* thisptr,
                                                        CMqPoint* screenSurfaceSize);
    GetScreenSurfaceSize getScreenSurfaceSize;

    /**
     * Assumption: adds scaling start operation to the render queue if rendering is in process.
     * Otherwise immediately starts scaling in IMqRasterizer.
     * Does nothing if rasterizer does not support scaling.
     */
    using StartScaling = void(__thiscall*)(IMqDisplay2* thisptr,
                                           const CMqRect* area,
                                           const CMqPoint* position);
    StartScaling startScaling;

    /**
     * Assumption: adds scaling end operation to the render queue if rendering is in process.
     * Otherwise immediately ends scaling in IMqRasterizer.
     * Does nothing if rasterizer does not support scaling.
     */
    using EndScaling = void(__thiscall*)(IMqDisplay2* thisptr);
    EndScaling endScaling;

    /** Returns true if scaling operations are supported by current IMqRasterizer. */
    using IsScalingSupported = bool(__thiscall*)(const IMqDisplay2* thisptr);
    IsScalingSupported isScalingSupported;

    /** Returns true if game is in full screen mode. */
    using IsFullscreen = bool(__thiscall*)(const IMqDisplay2* thisptr);
    IsFullscreen isFullscreen;

    /**
     * Swaps screen buffers.
     * All implementations call IDirectDraw7::FlipToGDISurface.
     */
    using FlipToGDISurface = void(__thiscall*)(IMqDisplay2* thisptr);
    FlipToGDISurface flipToGDISurface;

    /** Returns true if specified display settings are supported. */
    using IsDisplaySettingsSupported = bool(__thiscall*)(IMqDisplay2* thisptr,
                                                         const DisplaySettings* settings);
    IsDisplaySettingsSupported isDisplaySettingsSupported;

    /** Initializes render surfaces. */
    using InitializeSurfaces = bool(__thiscall*)(IMqDisplay2* thisptr,
                                                 const DisplaySettings* settings);
    InitializeSurfaces initializeSurfaces;

    using Method10 = bool(__thiscall*)(const IMqDisplay2* thisptr, int a2);
    Method10 method10;
};

assert_vftable_size(IMqDisplay2Vftable, 11);

} // namespace game

#endif // MQDISPLAY2_H
