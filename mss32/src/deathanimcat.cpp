/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "deathanimcat.h"
#include "version.h"
#include <array>

namespace game::DeathAnimCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LDeathAnimCategory*)0x83a2b8,
        (LDeathAnimCategory*)0x83a2c8,
        (LDeathAnimCategory*)0x83a2d8,
        (LDeathAnimCategory*)0x83a2e8,
        (LDeathAnimCategory*)0x83a2f8,
        (LDeathAnimCategory*)0x83a308,
        (LDeathAnimCategory*)0x83a318,
        (LDeathAnimCategory*)0x83a330,
    },
    // Russobit
    Categories{
        (LDeathAnimCategory*)0x83a2b8,
        (LDeathAnimCategory*)0x83a2c8,
        (LDeathAnimCategory*)0x83a2d8,
        (LDeathAnimCategory*)0x83a2e8,
        (LDeathAnimCategory*)0x83a2f8,
        (LDeathAnimCategory*)0x83a308,
        (LDeathAnimCategory*)0x83a318,
        (LDeathAnimCategory*)0x83a330,
    },
    // Gog
    Categories{
        (LDeathAnimCategory*)0x838268,
        (LDeathAnimCategory*)0x838278,
        (LDeathAnimCategory*)0x838288,
        (LDeathAnimCategory*)0x838298,
        (LDeathAnimCategory*)0x8382a8,
        (LDeathAnimCategory*)0x8382b8,
        (LDeathAnimCategory*)0x8382c8,
        (LDeathAnimCategory*)0x8382e0,
    },
    // Scenario Editor
    Categories{
        (LDeathAnimCategory*)0x665c38,
        (LDeathAnimCategory*)0x665c48,
        (LDeathAnimCategory*)0x665c58,
        (LDeathAnimCategory*)0x665c68,
        (LDeathAnimCategory*)0x665c78,
        (LDeathAnimCategory*)0x665c88,
        (LDeathAnimCategory*)0x665c98,
        (LDeathAnimCategory*)0x665cb0,
    },
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::DeathAnimCategories
