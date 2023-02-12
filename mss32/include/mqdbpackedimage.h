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

#ifndef MQDBPACKEDIMAGE_H
#define MQDBPACKEDIMAGE_H

#include "d2string.h"
#include "d2vector.h"
#include "mqpoint.h"
#include <Windows.h>
#include <cstdint>

namespace game {

/**
 * Describes part of a packed image.
 * Packed images contain shuffled rectangular areas (parts).
 * Each part is described by MQDBImagePart structure that can be found inside '-IMAGES.OPT'.
 * Areas described as top-left corner and size.
 */
struct MQDBImagePart
{
    std::uint32_t sourceX; /**< X coordinate of shuffled image part */
    std::uint32_t sourceY; /**< Y coordinate of shuffled image part */
    std::uint32_t targetX; /**< X coordinate of part in final image */
    std::uint32_t targetY; /**< Y coordinate of part in final image */
    std::uint32_t width;   /**< Width of image part */
    std::uint32_t height;  /**< Height of image part */
};

assert_size(MQDBImagePart, 24);

struct MQDBImageFrame
{
    String name;
    std::uint32_t width;
    std::uint32_t height;
    Vector<MQDBImagePart> parts;
};

assert_size(MQDBImageFrame, 40);

/**
 * Describes packed image or animation.
 * Simple packed image will contain a single frame that describes its contents,
 * whereas animation would contain multiple frames.
 * All animation frames must have the same width and height to be read correctly by the game.
 */
struct MQDBPackedImage
{
    ::tagPALETTEENTRY palette[256];
    CMqPoint size;
    std::uint8_t transparentColor;
    char padding;
    std::uint16_t opacity;
    Vector<MQDBImageFrame> frames;
    bool flipped;
    char padding2[3];
};

assert_size(MQDBPackedImage, 1056);

} // namespace game

#endif // MQDBPACKEDIMAGE_H
