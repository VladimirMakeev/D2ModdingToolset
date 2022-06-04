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

#ifndef MQTEXTURER2_H
#define MQTEXTURER2_H

#include <cstdint>
#include <windows.h>

namespace game {

struct IMqTexturer2Vftable;
struct TextureHandle;
struct IMqTexture;
struct CMqPoint;

/** Statistics are shown in CMqFps window. */
struct TexturerStatistics
{
    int unknown; /**< Number of elements in CRendererImpl::data40IntPair.first. */
    int surface;
    int bytes;
    int video;
};

static_assert(sizeof(TexturerStatistics) == 16,
              "Size of TexturerStatistics structure must be exactly 16 bytes");

struct IMqTexturer2
{
    IMqTexturer2Vftable* vftable;
};

// Virtual table does not contain destructor
struct IMqTexturer2Vftable
{
    using SetBool = bool(__thiscall*)(IMqTexturer2* thisptr, bool value);

    SetBool setUnknown;

    /**
     * Setting related to low-end videocards (from game era).
     * Called when player sets 'High quality translucency effects' in options menu
     * or 'HighAlpha' in Disciple.ini to low.
     */
    SetBool setLowAlpha;

    /**
     * Meaning assumed.
     * Adds texture handle and checks there is no palette entries associated with specified key.
     */
    using AddTextureCheckPalette = TextureHandle*(__thiscall*)(IMqTexturer2* thisptr,
                                                               TextureHandle* textureHandle,
                                                               IMqTexture* texture,
                                                               CMqPoint* size,
                                                               std::uint32_t key,
                                                               int type,
                                                               std::uint16_t opacity);
    AddTextureCheckPalette addTextureCheckPalette;

    /**
     * Meaning assumed.
     * Adds texture handle. Called from constructor of CMqImage2Surface16, for example.
     */
    using AddTexture = TextureHandle*(__thiscall*)(IMqTexturer2* thisptr,
                                                   TextureHandle* textureHandle,
                                                   IMqTexture* texture,
                                                   const CMqPoint* size,
                                                   int type,
                                                   std::uint16_t opacity);
    AddTexture addTexture;

    /**
     * Meaning assumed.
     * Removes texture handle. Called from destructor of CMqImage2Surface16, for example.
     */
    using RemoveTexture = void(__thiscall*)(IMqTexturer2* thisptr, TextureHandle* textureHandle);
    RemoveTexture removeTexture;

    /** Returns texture by specified handle. */
    using GetTexture = IMqTexture*(__thiscall*)(IMqTexturer2* thisptr, int a2);
    GetTexture getTexture;

    /**
     * Checks and restores all lost surfaces found by specified handle,
     * then forces redraw of IMqTextures found by key from handle.
     * Same underlying logic is called from IMqRenderer2::method3.
     */
    using Method6 = void(__thiscall*)(IMqTexturer2* thisptr, int a2);
    Method6 method6;

    using GetColorBitMask = void(__thiscall*)(IMqTexturer2* thisptr,
                                              std::uint32_t* red,
                                              std::uint32_t* green,
                                              std::uint32_t* blue,
                                              std::uint32_t* alpha);
    GetColorBitMask getColorBitMask;

    /**
     * Adds palette entries with specified key.
     * Generates unique key if provided key is 0.
     */
    using AddPaletteEntries = std::uint32_t(__thiscall*)(IMqTexturer2* thisptr,
                                                         std::uint32_t key,
                                                         const PALETTEENTRY* entries);
    AddPaletteEntries addPaletteEntries;

    /**
     * Fills array with 256 palette entries found by specified key.
     * @returns true if entries were found.
     */
    using GetPaletteEntries = bool(__thiscall*)(const IMqTexturer2* thisptr,
                                                std::uint32_t key,
                                                PALETTEENTRY* paletteEntries);
    GetPaletteEntries getPaletteEntries;

    /**
     * Removes palette entries by specified key.
     * Removes palette if there is no textures that uses it.
     * @returns true if entries were removed.
     */
    using RemovePalette = bool(__thiscall*)(IMqTexturer2* thisptr, std::uint32_t key);
    RemovePalette removePalette;

    /**
     * Clears texture cache.
     * Called from CMidgardOnKeyPressed when pressing Ctrl+E.
     */
    using ClearTextureCache = int(__thiscall*)(IMqTexturer2* thisptr);
    ClearTextureCache clearTextureCache;

    /** Returns current texturer statistics. */
    using GetTexturerStats = void(__thiscall*)(const IMqTexturer2* thisptr,
                                               TexturerStatistics* texturerStats);
    GetTexturerStats getTexturerStats;

    using Method13 = void(__thiscall*)(IMqTexturer2* thisptr, void* a2, void* a3);
    Method13 method13;
};

static_assert(sizeof(IMqTexturer2Vftable) == 14 * sizeof(void*),
              "IMqTexturer2 vftable must have exactly 14 methods");

} // namespace game

#endif // MQTEXTURER2_H
