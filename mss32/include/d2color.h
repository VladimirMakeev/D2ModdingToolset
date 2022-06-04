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

#ifndef D2COLOR_H
#define D2COLOR_H

#include "d2assert.h"
#include <cstdint>

namespace game {

/** Representation of a color in game. */
struct Color
{
    constexpr Color() = default;

    /** ABGR format. */
    constexpr Color(std::uint32_t value)
        : value{value}
    { }

    constexpr Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 0)
        : r{r}
        , g{g}
        , b{b}
        , a{a}
    { }

    union
    {
        struct
        {
            std::uint8_t r;
            std::uint8_t g;
            std::uint8_t b;
            std::uint8_t a; /**< Assumption, no use cases found yet. */
        };

        std::uint32_t value{};
    };
};

assert_size(Color, 4);

} // namespace game

#endif // D2COLOR_H
