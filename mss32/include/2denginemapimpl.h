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

#ifndef C2DENGINEMAPIMPL_H
#define C2DENGINEMAPIMPL_H

#include "2denginemap.h"
#include "d2map.h"
#include "d2vector.h"
#include "mqpoint.h"
#include "smartptr.h"

namespace game {

struct C2DEngine;
struct CUIManager;

using ElementIndexHashMap = Map<int /* elementIndex */,
                                int /* isoLayerPositionHash */,
                                SmartPointer>;

assert_size(ElementIndexHashMap, 36);

struct EngineMapImageElementData
{
    IMqImage2* image;
    CMqPoint position;
    CMqPoint imageSize;
    int elementIndex;
};

assert_size(EngineMapImageElementData, 24);

using ElementDataArray = Vector<EngineMapImageElementData, SmartPointer>;

assert_size(ElementDataArray, 20);

struct EngineMapImageElement
{
    ElementDataArray dataElements;
    /**
     * Hash value computed as:
     * @code{.cpp}isoLayer + ((position->x + position->y + (isoLayer >> 9 << 8)) << 9);@endcode
     */
    int isoLayerPositionHash;
    int unknown2;
    int unknown3;
    int unknown4;
    int unknown5;
    bool dirty; /**< Meaning assumed. */
    char padding[3];
};

assert_size(EngineMapImageElement, 44);

struct C2DEngineMapImplData
{
    C2DEngine* engine2d;
    SmartPtr<CUIManager> uiManagerPtr;
    Pair<ElementIndexHashMap, int /* freeElementIndex */> elementHashMapFreeIndexPair;
    Vector<EngineMapImageElement, SmartPointer> imageElements;
    Pair<Vector<char[16]>, int> unknownData;
    bool dirty; /**< Meaning assumed. */
    char padding[3];
    char unknown[24];
};

assert_size(C2DEngineMapImplData, 120);

struct C2DEngineMapImpl : public C2DEngineMap
{
    C2DEngineMapImplData* data;
};

assert_size(C2DEngineMapImpl, 8);

} // namespace game

#endif // C2DENGINEMAPIMPL_H
