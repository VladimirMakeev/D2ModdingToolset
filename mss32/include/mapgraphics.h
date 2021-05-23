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

#include "linkedlist.h"
#include "smartptr.h"
#include <cstddef>

namespace game {

struct C2DEngine;
struct CIsoEngineGround;

struct MapGraphics
{
    C2DEngine* engine2d;
    char unknown[54];
    CIsoEngineGround* isoEngineGround;
    char unknown2[8];
    LinkedList<void*> list;
    int unknown3;
    SmartPointer ptr;
};

static_assert(sizeof(MapGraphics) == 100,
              "Size of MapGraphics structure must be exactly 100 bytes");

static_assert(offsetof(MapGraphics, isoEngineGround) == 60,
              "MapGraphics::isoEngineGround offset must be 60 bytes");

static_assert(offsetof(MapGraphics, list) == 72, "MapGraphics::list offset must be 72 bytes");

static_assert(offsetof(MapGraphics, ptr) == 92, "MapGraphics::ptr offset must be 92 bytes");

using MapGraphicsPtr = SmartPtr<MapGraphics*>;

namespace MapGraphicsApi {

struct Api
{
    using GetMapGraphics = MapGraphicsPtr*(__stdcall*)(MapGraphicsPtr* ptr);
    GetMapGraphics getMapGraphics;

    using SetMapGraphics = void(__thiscall*)(MapGraphicsPtr* thisptr, MapGraphics** data);
    SetMapGraphics setMapGraphics;

    using GetTileIndex = int(__thiscall*)(MapGraphics** thisptr, bool waterTile);
    GetTileIndex getTileIndex;

    using StoreBlackTiles = int(__thiscall*)(MapGraphics** thisptr, int blackTilesIndex);
    StoreBlackTiles storeBlackTiles;
};

Api& get();

} // namespace MapGraphicsApi

} // namespace game

#endif // MAPGRAPHICS_H
