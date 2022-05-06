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

#ifndef MQRENDERER2_H
#define MQRENDERER2_H

namespace game {

struct IMqRenderer2Vftable;
struct CMqPoint;
struct CMqRect;
struct TextureHandle;

/** Statistics are shown in CMqFps window. */
struct RenderStatistics
{
    int unique;
    int used;
    int created;
    int loaded;
    int texels;
    int pixels;
    int memory;
    int blitCount;
};

static_assert(sizeof(RenderStatistics) == 32,
              "Size of RenderStatistics structure must be exactly 32 bytes");

struct IMqRenderer2
{
    IMqRenderer2Vftable* vftable;
};

// Virtual table does not contain destructor
struct IMqRenderer2Vftable
{
    using Method0 = int(__thiscall*)(IMqRenderer2* thisptr);
    Method0 method0;

    /** Returns current fps the game is running on. */
    using GetFps = float(__thiscall*)(const IMqRenderer2* thisptr);
    GetFps getFPS;

    /**
     * Assumption: pre-render.
     * Called before drawing CInterface elements.
     */
    using Method2 = void(__thiscall*)(IMqRenderer2* thisptr);
    Method2 method2;

    /**
     * Assumption: main rendering logic.
     * Called after drawing CInterface elements.
     * Calls IMqTexture methods to draw their contents. Performs alpha blending.
     * Swaps front and back buffers, updates fps and render statistics.
     */
    using Method3 = int(__thiscall*)(IMqRenderer2* thisptr);
    Method3 method3;

    using Method4 = int(__thiscall*)(IMqRenderer2* thisptr,
                                     TextureHandle* textureHandle,
                                     const CMqPoint* start,
                                     const CMqPoint* offset,
                                     const CMqPoint* size,
                                     const CMqRect* area);
    Method4 method4;

    void* method5;
    void* method6;

    /** Returns current renderer statistics. */
    using GetRenderStats = void(__thiscall*)(const IMqRenderer2* thisptr,
                                             RenderStatistics* statistics);
    GetRenderStats getRenderStats;

    using Method8 = void(__thiscall*)(IMqRenderer2* thisptr);
    Method8 method8;

    using Method9 = void(__thiscall*)(IMqRenderer2* thisptr, int a2, int a3, int a4);
    Method9 method9;

    void* method10;
    void* method11;

    using Method12 = void(__thiscall*)(IMqRenderer2* thisptr);
    Method12 method12;
};

static_assert(sizeof(IMqRenderer2Vftable) == 13 * sizeof(void*),
              "IMqRenderer2 vftable must have exactly 13 methods");

} // namespace game

#endif // MQRENDERER2_H
