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

#ifndef IMAGE2MEMORY_H
#define IMAGE2MEMORY_H

#include "d2color.h"
#include "mqimage2surface16.h"
#include <vector>

namespace hooks {

/** Represents image in memory that does not depend on .ff files. */
struct CImage2Memory : public game::CMqImage2Surface16
{
    CImage2Memory(std::uint32_t width, std::uint32_t height);

    std::vector<game::Color> pixels;
};

CImage2Memory* createImage2Memory(std::uint32_t width, std::uint32_t height);

} // namespace hooks

#endif // IMAGE2MEMORY_H
