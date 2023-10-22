/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#ifndef DISPLAYDDRAW_H
#define DISPLAYDDRAW_H

#include "d2pair.h"
#include "displaysettings.h"
#include "mqrasterizer.h"
#include <ddraw.h>

namespace game {

using DirectDrawSurfaceDescPair = Pair<IDirectDrawSurface7*, _DDSURFACEDESC2>;

struct CLogFile;

struct CDisplayDDraw : public IMqRasterizer
{
    IDirectDraw7* directDraw;
    IDirectDrawSurface7* directDrawSurface;
    IDirectDrawSurface7* directDrawSurfaceScreen;
    IDirectDrawSurface7* surface1152;
    IDirectDrawSurface7* surfaceFromScreen;
    bool alwaysTrue;
    bool lowAlpha;
    char padding[2];
    DirectDrawSurfaceDescPair surfaceDescPair;
    bool surfaceDescPairLocked;
    char padding4[3];
    DirectDrawSurfaceDescPair surfaceDescPair2;
    bool surfaceDescPair2Locked;
    char padding5[3];
    int unk3;
    char unknown[48];
    DisplaySettings displayMode;
    bool fullscreen;
    char padding2[3];
    int refreshRate;
    HWND hWnd;
    int screenSurfaceWidth;
    int screenSurfaceHeight;
    bool unk4;
    char padding3[3];
    int unk5;
    CLogFile* logFile;
};

assert_size(CDisplayDDraw, 388);

namespace CDisplayDDrawApi {

const IMqRasterizerVftable* vftable();

}

} // namespace game

#endif // DISPLAYDDRAW_H
