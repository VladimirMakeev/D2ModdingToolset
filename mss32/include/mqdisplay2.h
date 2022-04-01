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

namespace game {

struct IMqDisplay2Vftable;
struct CMqPoint;
struct DisplaySettings;

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

    using Method3 = int(__thiscall*)(IMqDisplay2* thisptr, int a2, int a3);
    Method3 method3;

    using Method4 = int(__thiscall*)(IMqDisplay2* thisptr);
    Method4 method4;

    using Method5 = bool(__thiscall*)(IMqDisplay2* thisptr);
    Method5 method5;

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

static_assert(sizeof(IMqDisplay2Vftable) == 11 * sizeof(void*),
              "IMqDisplay2 vftable must have exactly 11 methods");

} // namespace game

#endif // MQDISPLAY2_H
