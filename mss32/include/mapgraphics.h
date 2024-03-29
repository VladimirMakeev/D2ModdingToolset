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

#ifndef MAPGRAPHICS_H
#define MAPGRAPHICS_H

#include "d2list.h"
#include "d2map.h"
#include "functordispatch1.h"
#include "mqrect.h"
#include "smartptr.h"
#include "tileindices.h"
#include <cstddef>

namespace game {

struct C2DEngine;
struct CIsoEngineGround;
struct IIsoCBScroll;
struct IMqImage2;
struct CIsoLayer;

/**
 * Maps iso layer and map tile positions to 2d engine element indices.
 * Hash value computed as:
 * @code{.cpp}
 * (layerNumber << 20)
 *         | ((mapPosX & 0xFFF003FF | ((mapPosX & 0x3FF) << 10))
 *            ^ (mapPosY ^ (mapPosX & 0xFFF003FF | ((mapPosX & 0x3FF) << 10))) & 0x3FF)
 *               & 0xFFFFF;
 * @endcode
 * Element indices are used by C2DEngineMapImpl to add, remove and change map elements images.
 */
using HashElementIndexMap = Map<int /* hash */, int /* elementIndex */, SmartPointer>;

assert_size(HashElementIndexMap, 36);

struct MapGraphics
{
    C2DEngine* engine2d;
    CMqRect isoViewArea; /**< Area of isometric map view. */
    int mapSize;
    HashElementIndexMap hashElementIndexMap;
    CIsoEngineGround* isoEngineGround;
    int scrollSpeed;
    bool unknown5;
    char padding[3];
    List<IIsoCBScroll*> isoScrollList;
    bool unknown6;
    char padding2[3];
    /**
     * Callback that returns true if mouse coordinates can be converted to tile coordinates.
     * Set by CStratInterf in its c-tor.
     */
    SmartPtr<CBFunctorDispatch1wRet<const CMqPoint*, bool>> canConvertMouse;
};

assert_size(MapGraphics, 100);
assert_offset(MapGraphics, isoEngineGround, 60);
assert_offset(MapGraphics, isoScrollList, 72);
assert_offset(MapGraphics, canConvertMouse, 92);

using MapGraphicsPtr = SmartPtr<MapGraphics*>;

namespace MapGraphicsApi {

struct Api
{
    using GetMapGraphics = MapGraphicsPtr*(__stdcall*)(MapGraphicsPtr* ptr);
    GetMapGraphics getMapGraphics;

    using SetMapGraphics = void(__thiscall*)(MapGraphicsPtr* thisptr, MapGraphics** data);
    SetMapGraphics setMapGraphics;

    using GetTileIndex = TileArrayIndex(__thiscall*)(MapGraphics** thisptr, bool waterTile);
    GetTileIndex getTileIndex;

    /** Sets tile array index used for tiles outside of map bounds. */
    using SetOutOfBordersTileIndex = int(__thiscall*)(MapGraphics** thisptr,
                                                      TileArrayIndex tileIndex);
    SetOutOfBordersTileIndex setOutOfBordersTileIndex;

    /**
     * Shows specified image on selected layer.
     * Image is placed according to mapPosition.
     */
    using ShowImageOnMap = void(__stdcall*)(const CMqPoint* mapPosition,
                                            const CIsoLayer* layer,
                                            const IMqImage2* image,
                                            int a4,
                                            int a5);
    ShowImageOnMap showImageOnMap;

    /** Hides all images on selected layer. */
    using HideLayerImages = void(__stdcall*)(const CIsoLayer* layer);
    HideLayerImages hideLayerImages;
};

Api& get();

} // namespace MapGraphicsApi

} // namespace game

#endif // MAPGRAPHICS_H
