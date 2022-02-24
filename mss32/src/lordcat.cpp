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

#include "lordcat.h"
#include "version.h"
#include <array>

namespace game::LordCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LLordCategory*)0x8397a8,
        (LLordCategory*)0x839798,
        (LLordCategory*)0x8397c0,
    },
    // Russobit
    Categories{
        (LLordCategory*)0x8397a8,
        (LLordCategory*)0x839798,
        (LLordCategory*)0x8397c0,
    },
    // Gog
    Categories{
        (LLordCategory*)0x837758,
        (LLordCategory*)0x837748,
        (LLordCategory*)0x837770,
    },
    // Scenario Editor
    Categories{
        (LLordCategory*)0x664fb8,
        (LLordCategory*)0x664fa8,
        (LLordCategory*)0x664fd0,
    },
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::LordCategories
