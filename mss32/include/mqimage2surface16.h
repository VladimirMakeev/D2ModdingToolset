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

#ifndef MQIMAGE2SURFACE16_H
#define MQIMAGE2SURFACE16_H

#include "mqimage2.h"
#include "mqtexture.h"
#include "smartptr.h"
#include "texturehandle.h"
#include <cstddef>

namespace game {

struct CMqPresentationManager;

struct CMqImage2Surface16
    : public IMqImage2
    , public IMqTexture
{
    SmartPtr<CMqPresentationManager> presentationMgr;
    TextureHandle textureHandle;
    CMqPoint size;
    bool dirty;
    char padding[3];
};

assert_size(CMqImage2Surface16, 44);
assert_offset(CMqImage2Surface16, size, 32);

namespace CMqImage2Surface16Api {

struct Api
{
    using Constructor = CMqImage2Surface16*(__thiscall*)(CMqImage2Surface16* thisptr,
                                                         std::uint32_t width,
                                                         std::uint32_t height,
                                                         int hint,
                                                         int opacity);
    Constructor constructor;
};

Api& get();

} // namespace CMqImage2Surface16Api

} // namespace game

#endif // MQIMAGE2SURFACE16_H
