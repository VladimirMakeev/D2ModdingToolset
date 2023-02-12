/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#ifndef FACEIMGIMPL_H
#define FACEIMGIMPL_H

#include "faceimg.h"
#include "mqtexture.h"
#include "smartptr.h"
#include "texturehandle.h"

namespace game {

struct CMqPresentationManager;
struct ImageDataDescriptor;

struct CFaceImgImpl
    : public CFaceImg::IFaceImg
    , public IMqTexture
{
    SmartPtr<CMqPresentationManager> presentationManager;
    TextureHandle textureHandle;
    ImageDataDescriptor* imageDataDescriptor;
    char unknown36[32];
    int unknown68;
    int percentHp;
    bool leftSide;
    bool unknown77;
    bool unknown78;
    char padding;
};

assert_size(CFaceImgImpl, 80);
assert_offset(CFaceImgImpl, unknown36, 36);
assert_offset(CFaceImgImpl, unknown68, 68);

} // namespace game

#endif // FACEIMGIMPL_H
