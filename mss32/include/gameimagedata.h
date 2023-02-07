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

#ifndef GAMEIMAGEDATA_H
#define GAMEIMAGEDATA_H

#include "d2assert.h"
#include "imgfactory.h"

namespace game {

struct MQDBDataWrapper;
struct MQDBImageDataWrapper;
struct CFFFactory;
struct ImgPackerStoreLoaderWrapper;

struct GameImageData
{
    MQDBDataWrapper* imagesData;
    MQDBImageDataWrapper* unkImgData2;
    CImgFactory::FactoryLoader* ffFactory;
    ImgPackerStoreLoaderWrapper* packerStoreLoader;
    IdManagerSmartPtr idManager;
};

assert_size(GameImageData, 24);

struct GameImageDataWrapper
{
    GameImageData* data;
};

assert_size(GameImageDataWrapper, 4);

} // namespace game

#endif // GAMEIMAGEDATA_H
