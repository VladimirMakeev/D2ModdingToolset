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

#include "d2assert.h"
#include "d2pair.h"
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

struct ImgStore2LoaderUnordMapRecordData
{
    Pair<int /* imageId */, bool /* Assumption: flipped image flag */> keyPair;
    ImgStore2LoaderUnordMapData data;
};

assert_size(ImgStore2LoaderUnordMapRecordData, 40);

struct ImgStore2LoaderUnordMapRecord
{
    ImgStore2LoaderUnordMapRecordData data;
    std::uint32_t hash;
    ImgStore2LoaderUnordMapRecord* next;
};

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

struct ImgStore2LoaderUnordMap
{
    char unknown;
    char padding[3];
    std::uint32_t recordsTotal;
    ImgStore2LoaderUnordMapRecord** records;
    std::uint32_t recordsAllocated;
    int unknown2;
    int unknown3;
    char unknown4;
    char unknown5;
    char unknown6;
    char unknown7;
    int unknown8;
    int unknown9;
    void* allocator;
};

assert_size(ImgStore2LoaderUnordMap, 40);

struct ImgStore2Loader
{
    SmartPtr<CMqPresentationManager> presentationManager;
    void* memPool;
    CImgStore2LoaderWeakDup* store2Loader;
    ImgStore2LoaderUnordMap unordered_map;
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
