/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#ifndef IMGSTORE2LOADER_H
#define IMGSTORE2LOADER_H

#include "d2pair.h"
#include "d2unorderedmap.h"
#include "imgstore2.h"
#include "smartptr.h"
#include <cstdint>

namespace game {

struct CMqPresentationManager;
struct CImgStore2LoaderWeakDup;
struct TextureHandle;

struct ImgStore2LoaderUnordMapData
{
    int refCount;
    std::uint32_t ffImageClassSize;
    TextureHandle* textureHandle;
    int unknown4;
    int unknown5;
    int unknown6;
    CImgStore2::Image* ffImage;
    int imageId;
};

assert_size(ImgStore2LoaderUnordMapData, 32);

using ImageIdFlippedFlagPair = Pair<int /* imageId */, bool /* Assumption: flipped image flag */>;

using ImgStore2LoaderUnordMapRecord = UnorderedMapBucket<ImageIdFlippedFlagPair,
                                                         ImgStore2LoaderUnordMapData>;

assert_size(ImgStore2LoaderUnordMapRecord, 48);

struct ImgStore2LoaderUnordMapIterator
{
    bool unknown;
    char padding[3];
    ImgStore2LoaderUnordMapRecord** records;
    ImgStore2LoaderUnordMapRecord* record;
    std::uint32_t recordsTotal;
    bool valid;
    char padding2[3];
};

assert_size(ImgStore2LoaderUnordMapIterator, 20);

struct ImgStore2Loader
{
    SmartPtr<CMqPresentationManager> presentationManager;
    void* memPool;
    CImgStore2LoaderWeakDup* store2Loader;
    UnorderedMap<ImageIdFlippedFlagPair, ImgStore2LoaderUnordMapData> unordered_map;
    int unknown; /**< Maybe part of unordered_map */
};

assert_size(ImgStore2Loader, 60);

struct ImgStore2LoaderWrapper
{
    ImgStore2Loader* data;
};

assert_size(ImgStore2LoaderWrapper, 4);

} // namespace game

#endif // IMGSTORE2LOADER_H
