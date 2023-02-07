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

#ifndef IMAGEPACKERSTORELOADER_H
#define IMAGEPACKERSTORELOADER_H

#include "d2assert.h"

namespace game {

struct CImgFactoryAnimLoaderWeakDup;
struct ImgStore2LoaderWrapper;
struct ImgPackerLoaderWrapper;

struct ImgPackerStoreLoader
{
    CImgFactoryAnimLoaderWeakDup* animLoader;
    ImgStore2LoaderWrapper* store2Loader;
    ImgPackerLoaderWrapper* packerLoader;
};

assert_size(ImgPackerStoreLoader, 12);

struct ImgPackerStoreLoaderWrapper
{
    ImgPackerStoreLoader* data;
};

assert_size(ImgPackerStoreLoaderWrapper, 4);

} // namespace game

#endif // IMAGEPACKERSTORELOADER_H
