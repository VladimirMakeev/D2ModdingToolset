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

#ifndef C2DENGINEMAP_H
#define C2DENGINEMAP_H

#include "d2assert.h"

namespace game {

struct C2DEngineMapVftable;
struct IMqImage2;
struct CMqPoint;

struct C2DEngineMap
{
    C2DEngineMapVftable* vftable;
};

struct C2DEngineMapVftable
{
    using Destructor = void(__thiscall*)(C2DEngineMap* thisptr, char flags);
    Destructor destructor;

    /** Returns image by specified element index or nullptr if no image found. */
    using GetImage = IMqImage2*(__thiscall*)(C2DEngineMap* thisptr, int elementIndex);
    GetImage getImage;

    /**
     * Adds specified image with selected position as map element.
     * @param isoLayerPositionHash hash value computed as:
     * @code{.cpp}isoLayer + ((position->x + position->y + (isoLayer >> 9 << 8)) << 9);@endcode
     * @returns element index corresponding to the added element or -1 if element was not added.
     */
    using AddMapElement = int(__thiscall*)(C2DEngineMap* thisptr,
                                           const CMqPoint* position,
                                           int isoLayerPositionHash,
                                           IMqImage2* image);
    AddMapElement addMapElement;

    /**
     * Removes map element with specified element index.
     * @returns true if element was found and removed.
     */
    using RemoveMapElement = bool(__thiscall*)(C2DEngineMap* thisptr, int elementIndex);
    RemoveMapElement removeMapElement;

    /**
     * Changes map element with specified element index by replacing its hash and position.
     * @returns true if element was changed.
     */
    using ChangeMapElement = bool(__thiscall*)(C2DEngineMap* thisptr,
                                               int elementIndex,
                                               int isoLayerPositionHash,
                                               const CMqPoint* position);
    ChangeMapElement changeMapElement;

    /**
     * Removes all map elements with specified hash value.
     * Calls RemoveMapElement on each found element.
     */
    using RemoveElements = void(__thiscall*)(C2DEngineMap* thisptr, int isoLayerPositionHash);
    RemoveElements removeElements;

    /** Clears all map elements. */
    using Clear = void(__thiscall*)(C2DEngineMap* thisptr);
    Clear clear;
};

assert_vftable_size(C2DEngineMapVftable, 7);

} // namespace game

#endif // C2DENGINEMAP_H
