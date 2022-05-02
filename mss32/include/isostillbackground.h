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

#ifndef ISOSTILLBACKGROUND_H
#define ISOSTILLBACKGROUND_H

namespace game {

struct IIsoStillBackgroundVftable;
struct SurfaceDecompressData;

struct IIsoStillBackground
{
    IIsoStillBackgroundVftable* vftable;
};

static_assert(sizeof(IIsoStillBackground) == 4,
              "Size of IIsoStillBackground structure must be exactly 4 bytes");

struct IIsoStillBackgroundVftable
{
    using Destructor = void(__thiscall*)(IIsoStillBackground* thisptr, char flags);
    Destructor destructor;

    using Draw = void(__thiscall*)(IIsoStillBackground* thisptr,
                                   SurfaceDecompressData* surfaceDecompress);
    Draw draw;
};

static_assert(sizeof(IIsoStillBackgroundVftable) == 2 * sizeof(void*),
              "IIsoStillBackground vftable must have exactly 2 methods");

struct CIsoStillBackground : public IIsoStillBackground
{
    void*** imageData;
};

static_assert(sizeof(CIsoStillBackground) == 8,
              "Size of CIsoStillBackground structure must be exactly 8 bytes");

} // namespace game

#endif // ISOSTILLBACKGROUND_H
