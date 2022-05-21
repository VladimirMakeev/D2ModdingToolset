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

#ifndef RENDERERIMPL_H
#define RENDERERIMPL_H

#include "mqanimator2.h"
#include "mqdisplay2.h"
#include "mqrenderer2.h"
#include "mqtexturer2.h"
#include "rendertypes.h"
#include "uievent.h"

namespace game {

struct CRendererImplDisplayVftable;
struct IMqRasterizerFactory;
struct IMqRasterizer;
struct CUIManager;
struct CLogFile;
struct Animations;

struct CRendererImpl
    : public IMqDisplay2T<CRendererImplDisplayVftable>
    , public IMqRenderer2
    , public IMqTexturer2
    , public IMqAnimator2
{
    IMqRasterizerFactory* rasterizerFactory;
    IMqRasterizer* rasterizer;
    SmartPtr<CUIManager> uiManager;
    UiEvent destroyEvent;
    int unknown3;
    int unknown4;
    LARGE_INTEGER perfFrequency;
    LARGE_INTEGER perfCounter;
    float fps;
    RECT windowClientArea;
    int unknown6;
    bool renderingInProcess;
    char padding[3];
    RenderData40SetIntPair** data40IntPair;
    Set<RenderPaletteData>** paletteData;
    Set<RenderData420>** data420;
    char unknown8;
    char padding2[3];
    int unknown9;
    Animations* animations;
    RenderStatistics renderStats;
    SmartPointer ptr;
    CLogFile* logFile;
};

static_assert(sizeof(CRendererImpl) == 176,
              "Size of CRendererImpl structure must be exactly 176 bytes");

static_assert(offsetof(CRendererImpl, CRendererImpl::IMqRenderer2::vftable) == 4,
              "Vftable offset for IMqRenderer2 in CRendererImpl structure must be 4 bytes");

static_assert(offsetof(CRendererImpl, CRendererImpl::IMqTexturer2::vftable) == 8,
              "Vftable offset for IMqTexturer2 in CRendererImpl structure must be 8 bytes");

static_assert(offsetof(CRendererImpl, CRendererImpl::IMqAnimator2::vftable) == 12,
              "Vftable offset for IMqAnimator2 in CRendererImpl structure must be 12 bytes");

static_assert(offsetof(CRendererImpl, rasterizer) == 20,
              "CRendererImpl::rasterizer offset must be 20 bytes");

static_assert(offsetof(CRendererImpl, fps) == 80, "CRendererImpl::fps offset must be 80 bytes");

static_assert(offsetof(CRendererImpl, data40IntPair) == 108,
              "CRendererImpl::data40IntPair offset must be 108 bytes");

struct CRendererImplDisplayVftable : public IMqDisplay2Vftable
{
    using Destructor = void(__thiscall*)(CRendererImpl* thisptr, char flags);
    Destructor destructor;
};

static_assert(sizeof(CRendererImplDisplayVftable) == 12 * sizeof(void*),
              "CRendererImpl vftable for IMqDisplay2 must have exactly 12 methods");

} // namespace game

#endif // RENDERERIMPL_H
