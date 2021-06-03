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

#include "groundcat.h"
#include "version.h"
#include <array>

namespace game::GroundCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LGroundCategory*)0x839360,
        (LGroundCategory*)0x839370,
        (LGroundCategory*)0x839380,
        (LGroundCategory*)0x839390,
    },
    // Russobit
    Categories{
        (LGroundCategory*)0x839360,
        (LGroundCategory*)0x839370,
        (LGroundCategory*)0x839380,
        (LGroundCategory*)0x839390,
    },
    // Gog
    Categories{
        (LGroundCategory*)0x837310,
        (LGroundCategory*)0x837320,
        (LGroundCategory*)0x837330,
        (LGroundCategory*)0x837340,
    },
    // Scenario Editor
    Categories{
        (LGroundCategory*)0x664e90,
        (LGroundCategory*)0x664ea0,
        (LGroundCategory*)0x664eb0,
        (LGroundCategory*)0x664ec0,
    },
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::GroundCategories
