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

#ifndef IISOGROUND2_H
#define IISOGROUND2_H

#include "d2assert.h"
#include <cstdint>

namespace game {

struct IIsoGround2Vftable;
struct SurfaceDecompressData;
struct CMqPoint;
struct CMqRect;

struct IIsoGround2
{
    IIsoGround2Vftable* vftable;
};

assert_size(IIsoGround2, 4);

struct IIsoGround2Vftable
{
    using Destructor = void(__thiscall*)(IIsoGround2* thisptr, char flags);
    Destructor destructor;

    using Draw = void(__thiscall*)(IIsoGround2* thisptr,
                                   SurfaceDecompressData* surfaceData,
                                   CMqPoint* srcPos,
                                   CMqPoint* dstPos,
                                   CMqRect* area,
                                   const std::uint32_t* blendMask,
                                   const std::uint32_t* alphaMask);
    Draw draw;

    using Method2 = void(__thiscall*)(IIsoGround2* thisptr,
                                      SurfaceDecompressData* surfaceData,
                                      CMqPoint* dstPos,
                                      CMqRect* textureArea);
    Method2 method2;
};

assert_vftable_size(IIsoGround2Vftable, 3);

} // namespace game

#endif // IISOGROUND2_H
