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

#ifndef FACTORYIMAGE2_H
#define FACTORYIMAGE2_H

#include "mqimage2.h"
#include "smartptr.h"

namespace game {

struct CMqPresentationManager;
struct TextureHandle;
struct ImgStore2LoaderWrapper;

struct FactoryImage2Data
{
    int imageId;
    bool flipped;
    char padding[3];
    TextureHandle* textureHandlePtr;
    ImgStore2LoaderWrapper* imgStoreLoaderWrapper;
};

assert_size(FactoryImage2Data, 16);

struct FactoryImage2 : IMqImage2
{
    SmartPtr<CMqPresentationManager> presentationMgr;
    FactoryImage2Data data;
    int unknown5;
};

assert_size(FactoryImage2, 32);

} // namespace game

#endif // FACTORYIMAGE2_H
