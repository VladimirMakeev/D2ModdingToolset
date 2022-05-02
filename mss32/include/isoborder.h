/* * This file is part of the modding toolset for Disciples 2.
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

#ifndef ISOBORDER_H
#define ISOBORDER_H

namespace game {

struct IIsoBorderVftable;
struct SurfaceDecompressData;
struct CMqPoint;
struct CMqRect;

struct IIsoBorder
{
    IIsoBorderVftable* vftable;
};

static_assert(sizeof(IIsoBorder) == 4, "Size of IIsoBorder structure must be exactly 4 bytes");

struct IIsoBorderVftable
{
    using Destructor = void(__thiscall*)(IIsoBorder* thisptr, char flags);
    Destructor destructor;

    using Draw = void(__thiscall*)(IIsoBorder* thisptr,
                                   SurfaceDecompressData* surfaceDecompress,
                                   CMqPoint* dstPos,
                                   CMqRect* area);
    Draw draw;

    /** Returns true if image has format of 8 bits per pixel. */
    using Is8BppImage = bool(__thiscall*)(const IIsoBorder* thisptr);
    Is8BppImage is8BppImage;

    /** Return type assumed from usage in CIsoEngineGround::CGroundTexture::Draw. */
    using GetWordData = const std::uint16_t*(__thiscall*)(const IIsoBorder* thisptr);
    GetWordData getWordData;

    /** Return type assumed from usage in CIsoEngineGround::CGroundTexture::Draw. */
    using GetByteData = const std::uint8_t*(__thiscall*)(const IIsoBorder* thisptr);
    GetByteData getByteData;
};

static_assert(sizeof(IIsoBorderVftable) == 5 * sizeof(void*),
              "IIsoBorder vftable must have exactly 5 methods");

} // namespace game

#endif // ISOBORDER_H
