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

#ifndef IMGSTORE2_H
#define IMGSTORE2_H

#include "d2set.h"
#include <Windows.h>

namespace game {

struct SurfaceDecompressData;
struct CMqPoint;

struct CImgStore2
{
    struct Store2LoaderVftable;

    struct Store2Loader
    {
        Store2LoaderVftable* vftable;
    };

    struct ImageVftable;

    struct Image
    {
        ImageVftable* vftable;
    };
};

struct CImgStore2::Store2LoaderVftable
{
    using Destructor = void(__thiscall*)(CImgStore2::Store2Loader* thisptr, char flags);
    Destructor destructor;

    using GetIndexOptRecordIds = Set<int>*(__thiscall*)(CImgStore2::Store2Loader* thisptr,
                                                        Set<int>* recordIds);
    GetIndexOptRecordIds getIndexOptRecordIds;

    using GetImageClassSize = std::uint32_t(__thiscall*)(CImgStore2::Store2Loader* thisptr,
                                                         const char* imageName,
                                                         int a2);
    GetImageClassSize getImageClassSize;

    /** Creates FFImg, for example. */
    using CreateImageObject = CImgStore2::Image*(__thiscall*)(CImgStore2::Store2Loader* thisptr,
                                                              int idValue,
                                                              int a2,
                                                              void* objectMemory);
    CreateImageObject createImageObject;
};

assert_vftable_size(CImgStore2::Store2LoaderVftable, 4);

struct CImgStore2::ImageVftable
{
    using Destructor = void(__thiscall*)(CImgStore2::Image* thisptr, char flags);
    Destructor destructor;

    using GetImageSize = CMqPoint*(__thiscall*)(CImgStore2::Image* thisptr, CMqPoint* size);
    GetImageSize getImageSize;

    using GetOpacity = std::uint16_t(__thiscall*)(const CImgStore2::Image* thisptr);
    GetOpacity getOpacity;

    using GetPalette = ::tagPALETTEENTRY*(__thiscall*)(CImgStore2::Image* thisptr,
                                                       ::tagPALETTEENTRY* entries);
    GetPalette getPalette;

    using Draw = void(__thiscall*)(CImgStore2::Image* thisptr,
                                   SurfaceDecompressData* decompressData);
    Draw draw;
};

assert_vftable_size(CImgStore2::ImageVftable, 5);

struct CImgStore2LoaderWeakDup : public CImgStore2::Store2Loader
{
    CImgStore2::Store2Loader* store2Loader;
};

assert_size(CImgStore2LoaderWeakDup, 8);

} // namespace game

#endif // IMGSTORE2_H
