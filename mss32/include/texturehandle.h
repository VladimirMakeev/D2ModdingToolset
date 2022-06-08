/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef TEXTUREHANDLE_H
#define TEXTUREHANDLE_H

#include "d2assert.h"
#include "mqpoint.h"
#include <cstdint>

namespace game {

/** Assumption: texture handle. */
struct TextureHandle
{
    CMqPoint textureSize;
    std::uint32_t* indexPtr; /**< Used as a key for search in CRendererImpl::textureSurfaceMap. */
    int* refCount;
};

assert_size(TextureHandle, 16);

} // namespace game

#endif // TEXTUREHANDLE_H
