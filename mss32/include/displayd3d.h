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

#ifndef DISPLAYD3D_H
#define DISPLAYD3D_H

#include "d2string.h"
#include "displaysettings.h"
#include "mqpoint.h"
#include "mqrasterizer.h"
#include "smartptr.h"
#include <ddraw.h>

struct IDirect3DDevice7;

namespace game {

struct CLogFile;

struct D3dDeviceInfo
{
    String devInfo;
    bool canRenderWindowed;
    bool drawPrimitives2ExtSupported;
    bool squareOnlyTextures;
    bool pow2TexturesRequired;
    bool nonLocalVideoMem;
    bool pageLockRequired;
    char padding[2];
    GUID deviceGuid;
    SmartPtr<GUID> driverGuid;
};

assert_size(D3dDeviceInfo, 48);

struct CDisplayD3DData
{
    IDirect3DDevice7* device;
    IDirectDrawSurface7* surface;
    IDirectDrawSurface7* attachedSurface;
    int colorKeyedTexturesSupported;
    DDPIXELFORMAT pixelFormat;
    bool alwaysFalse;
    char padding[3];
    CMqPoint maxTextureSize;
    D3dDeviceInfo deviceInfo;
    DisplaySettings displaySettings;
    bool isFullscreen;
    char padding2[3];
    int refreshRate;
    CMqPoint screenSurfaceSize;
    bool unknown7;
    bool unknown8;
    char padding3[2];
    CMqPoint areaSize;
    CMqPoint areaStart;
    CMqPoint position;
    IDirectDrawSurface7* anotherSurface;
    CMqPoint displaySize;
    bool scalingSupported;
    char padding4[3];
    HWND windowHandle;
    IDirectDrawSurface7* surface1152;
    IDirectDrawSurface7* pixelFormatSurface;
    bool unknown4;
    bool unknown4_1;
    char padding5[2];
    float vertexZ;
    int counter;
    CLogFile* logFile;
    int unknown5;
};

assert_size(CDisplayD3DData, 212);

struct CDisplayD3D : public IMqRasterizer
{
    CDisplayD3DData* data;
};

assert_size(CDisplayD3D, 8);

namespace CDisplayD3DApi {

struct Api
{
    using DrawPrimitive = void(__thiscall*)(CDisplayD3D* thisptr,
                                            IDirectDrawSurface7* surface,
                                            const CMqPoint* a3,
                                            const CMqPoint* position,
                                            const CMqPoint* areaSize,
                                            const CMqPoint* screenSurfaceSize,
                                            std::int16_t opacity);

    DrawPrimitive drawPrimitive;

    using SwapBuffers = void(__thiscall*)(CDisplayD3D* thisptr);
    SwapBuffers swapBuffers;

    using AlphaBlend = void(__thiscall*)(CDisplayD3D* thisptr,
                                         IDirectDrawSurface7* surface,
                                         const CMqPoint* a3,
                                         const CMqPoint* dstPosition,
                                         const CMqPoint* areaSize,
                                         int opacity);
    AlphaBlend alphaBlend;
};

Api& get();

} // namespace CDisplayD3DApi

} // namespace game

#endif // DISPLAYD3D_H
