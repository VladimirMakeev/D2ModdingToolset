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

#ifndef MQTEXTURE_H
#define MQTEXTURE_H

#include "d2assert.h"

namespace game {

struct IMqTextureVftable;
struct SurfaceDecompressData;

struct IMqTexture
{
    IMqTextureVftable* vftable;
};

struct IMqTextureVftable
{
    using Destructor = void(__thiscall*)(IMqTexture* thisptr, char flags);
    Destructor destructor;

    /**
     * Draws texture into decompressed surface memory.
     * Actual method signature unknown.
     */
    using Draw = void(__stdcall*)(IMqTexture* thisptr, SurfaceDecompressData* decompressData);
    Draw draw;

    /**
     * Returns true if surface contains changes and needs to be redrawn.
     * Actual method signature unknown.
     */
    using IsDirty = bool(__stdcall*)(const IMqTexture* thisptr);
    IsDirty isDirty;
};

assert_vftable_size(IMqTextureVftable, 3);

} // namespace game

#endif // MQTEXTURE_H
