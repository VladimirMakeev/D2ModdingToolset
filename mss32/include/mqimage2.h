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

#ifndef MQIMAGE2_H
#define MQIMAGE2_H

namespace game {

struct IMqImage2Vftable;
struct CMqPoint;
struct IMqRenderer;

/** Assumption: base class for all images. */
struct IMqImage2
{
    IMqImage2Vftable* vftable;
};

struct IMqImage2Vftable
{
    using Destructor = void(__thiscall*)(IMqImage2* thisptr, char flags);
    Destructor destructor;

    /** Returns image dimensions. */
    using GetSize = CMqPoint*(__thiscall*)(const IMqImage2* thisptr, CMqPoint* size);
    GetSize getSize;

    void* method2;
    void* method3;
    void* method4;

    /** Assumtion: renders image in CMqRect(start + offset, size) area. */
    using Render = void(__thiscall*)(const IMqImage2* thisptr,
                                     IMqRenderer* renderer,
                                     const CMqPoint* start,
                                     const CMqPoint* offset,
                                     const CMqPoint* size,
                                     int a6);
    Render render;

    void* method6;
    void* method7;
    void* method8;
};

static_assert(sizeof(IMqImage2Vftable) == 9 * sizeof(void*),
              "IMqImage2 vftable must have exactly 9 methods");

} // namespace game

#endif // MQIMAGE2_H
