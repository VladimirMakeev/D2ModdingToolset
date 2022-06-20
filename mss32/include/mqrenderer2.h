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

#include "d2assert.h"

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

assert_size(RenderStatistics, 32);

struct IMqRenderer2
{
    IMqRenderer2Vftable* vftable;
};

// Virtual table does not contain destructor
struct IMqRenderer2Vftable
{
    /** Assumption: returns current batch number. */
    using GetBatchNumber = int(__thiscall*)(IMqRenderer2* thisptr);
    GetBatchNumber getBatchNumber;

    /** Returns current fps the game is running on. */
    using GetFps = float(__thiscall*)(const IMqRenderer2* thisptr);
    GetFps getFPS;

    /**
     * Called before rendering frame.
     * Clears screen with solid black color.
     * Sets 'frame rendering' flag.
     * Called before drawing CMqPresentationManager::IPresentation elements.
     */
    using BeforeRender = void(__thiscall*)(IMqRenderer2* thisptr);
    BeforeRender beforeRender;

    /**
     * Main rendering logic.
     * Called after drawing all CMqPresentationManager::IPresentation elements.
     * Processes render queue into batch and calls IMqTexture methods
     * to draw their contents if dirty. Performs alpha blending.
     * Swaps front and back buffers, updates fps, animations and render statistics.
     * Resets 'frame rendering' flag.
     */
    using RenderFrame = int(__thiscall*)(IMqRenderer2* thisptr);
    RenderFrame renderFrame;

    /**
     * Adds texture handle with specified dimensions to render queue
     * for rendering in the current frame.
     * For example, called from CMqImage2Surface16::IMqImage2::render (0x670790).
     * CMqImage2Surface16 adds its texture handle along with points and area,
     * so render will pack this into a batch and call IMqTexture::draw during batch processing.
     * Does nothing if 'frame rendering' flag is not set.
     */
    using DrawTexture = void(__thiscall*)(IMqRenderer2* thisptr,
                                          TextureHandle* textureHandle,
                                          const CMqPoint* start,
                                          const CMqPoint* offset,
                                          const CMqPoint* size,
                                          const CMqRect* area);
    DrawTexture drawTexture;

    /**
     * Adds specified area to render queue list.
     * Use cases unknown.
     */
    using AddArea = void(__thiscall*)(IMqRenderer2* thisptr, const CMqRect* area);
    AddArea addArea;

    /**
     * Removes first area from render queue list.
     * Use cases unknown.
     */
    using RemoveArea = void(__thiscall*)(IMqRenderer2* thisptr);
    RemoveArea removeArea;

    /** Returns current renderer statistics. */
    using GetRenderStats = void(__thiscall*)(const IMqRenderer2* thisptr,
                                             RenderStatistics* statistics);
    GetRenderStats getRenderStats;

    /**
     * Clears render statistics, except blitCount.
     * Called from CMidgardOnKeyPressed when pressing Ctrl+R.
     */
    using ResetRenderStats = void(__thiscall*)(IMqRenderer2* thisptr);
    ResetRenderStats resetRenderStats;

    using Method9 = void(__thiscall*)(IMqRenderer2* thisptr, int a2, int a3, int a4);
    Method9 method9;

    void* method10;
    void* method11;

    using Method12 = void(__thiscall*)(IMqRenderer2* thisptr);
    Method12 method12;
};

assert_vftable_size(IMqRenderer2Vftable, 13);

} // namespace game

#endif // MQRENDERER2_H
