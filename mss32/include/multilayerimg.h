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

#ifndef MULTILAYERIMG_H
#define MULTILAYERIMG_H

#include "d2list.h"
#include "mqimage2.h"
#include "mqpoint.h"
#include "smartptr.h"

namespace game {

struct MultiLayerImgInfo
{
    SmartPtr<IMqImage2> image;
    int unknown;
    int unknown2;
    bool unknown3;
    bool unknown4;
    char padding[2];
};

static_assert(sizeof(MultiLayerImgInfo) == 20,
              "Size of MultiLayerImgInfo structure must be exactly 20 bytes");

struct CMultiLayerImgData
{
    List<MultiLayerImgInfo> images;
    CMqPoint size;
    std::uint32_t imagesCount;
};

static_assert(sizeof(CMultiLayerImgData) == 28,
              "Size of CMultiLayerImgData structure must be exactly 28 bytes");

/** Used to render several images on top of each other. */
struct CMultiLayerImg : public IMqImage2
{
    CMultiLayerImgData* data;
};

static_assert(sizeof(CMultiLayerImg) == 8,
              "Size of CMultiLayerImg structure must be exactly 8 bytes");

namespace CMultiLayerImgApi {

struct Api
{
    using Constructor = CMultiLayerImg*(__thiscall*)(CMultiLayerImg* thisptr);
    Constructor constructor;

    using AddImage = void(__thiscall*)(CMultiLayerImg* thisptr,
                                       const IMqImage2* image,
                                       int a3,
                                       int a4);
    AddImage addImage;
};

Api& get();

} // namespace CMultiLayerImgApi

} // namespace game

#endif // MULTILAYERIMG_H
