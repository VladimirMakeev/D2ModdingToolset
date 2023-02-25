/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#include "restrictions.h"
#include "version.h"
#include <array>

namespace game {

// clang-format off
std::array<GameRestrictions, 4> restrictions = {{
    // Akella
    GameRestrictions{
        (Restriction<int>*)0x6eb3f8,
        (Restriction<int>*)0x6ea928,
        (Restriction<int>*)0x6eb778,
        (Restriction<int>*)0x6ea954,
        (Restriction<int>*)0x6ea94c,
        (Restriction<int>*)0x6ea95c,
        (Restriction<int>*)0x6ea964,
        (Restriction<int>*)0x6ea944,
        (Restriction<int>*)0x6eb768,
        (Restriction<int>*)0x6eb770,
        (Restriction<int>*)0x6ea928,
        (Restriction<int>*)0x6ea934,
        (int*)0x6ea930,
        (std::uint32_t*)0x6eb780,
        (std::uint8_t*)0x65cefe,
        (std::uint8_t*)0x626726,
        (UnitHighLevels*)0x6ea9a8,
        {0, INT_MAX},
        {0, 100},
        {-100, 100},
    },
    // Russobit
    GameRestrictions{
        (Restriction<int>*)0x6eb3f8,
        (Restriction<int>*)0x6ea928,
        (Restriction<int>*)0x6eb778,
        (Restriction<int>*)0x6ea954,
        (Restriction<int>*)0x6ea94c,
        (Restriction<int>*)0x6ea95c,
        (Restriction<int>*)0x6ea964,
        (Restriction<int>*)0x6ea944,
        (Restriction<int>*)0x6eb768,
        (Restriction<int>*)0x6eb770,
        (Restriction<int>*)0x6ea928,
        (Restriction<int>*)0x6ea934,
        (int*)0x6ea930,
        (std::uint32_t*)0x6eb780,
        (std::uint8_t*)0x65cefe,
        (std::uint8_t*)0x626726,
        (UnitHighLevels*)0x6ea9a8,
        {0, INT_MAX},
        {0, 100},
        {-100, 100},
    },
    // Gog
    GameRestrictions{
        (Restriction<int>*)0x6e9398,
        (Restriction<int>*)0x6e88c8,
        (Restriction<int>*)0x6e9718,
        (Restriction<int>*)0x6e88f4,
        (Restriction<int>*)0x6e88ec,
        (Restriction<int>*)0x6e88fc,
        (Restriction<int>*)0x6e8904,
        (Restriction<int>*)0x6e88e4,
        (Restriction<int>*)0x6e9708,
        (Restriction<int>*)0x6e9710,
        (Restriction<int>*)0x6e88c8,
        (Restriction<int>*)0x6e88d4,
        (int*)0x6e88d0,
        (std::uint32_t*)0x6e9720,
        (std::uint8_t*)0x65b97e,
        (std::uint8_t*)0x625266,
        (UnitHighLevels*)0x6e8948,
        {0, INT_MAX},
        {0, 100},
        {-100, 100},
    },
    // Scenario Editor
    GameRestrictions{
        (Restriction<int>*)0x5df134,
        (Restriction<int>*)0x5de038,
        (Restriction<int>*)0x5df490,
        (Restriction<int>*)0x5de064,
        (Restriction<int>*)0x5de05c,
        (Restriction<int>*)0x5de06c,
        (Restriction<int>*)0x5de074,
        (Restriction<int>*)0x5de054,
        (Restriction<int>*)0x5df480,
        (Restriction<int>*)0x5df488,
        (Restriction<int>*)0x5de038,
        (Restriction<int>*)0x5de044,
        (int*)0x5de040,
        (std::uint32_t*)0x5df498,
        (std::uint8_t*)nullptr,
        (std::uint8_t*)nullptr,
        (UnitHighLevels*)0x5de0b8,
        {0, INT_MAX},
        {0, 100},
        {-100, 100},
    }
}};
// clang-format on

GameRestrictions& gameRestrictions()
{
    return restrictions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game
