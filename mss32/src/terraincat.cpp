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

#include "terraincat.h"
#include "version.h"
#include <array>

namespace game::TerrainCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LTerrainCategory*)0x8399c8,
        (LTerrainCategory*)0x8399b0,
        (LTerrainCategory*)0x839990,
        (LTerrainCategory*)0x8399a0,
        (LTerrainCategory*)0x839980,
        (LTerrainCategory*)0x839970,
    },
    // Russobit
    Categories{
        (LTerrainCategory*)0x8399c8,
        (LTerrainCategory*)0x8399b0,
        (LTerrainCategory*)0x839990,
        (LTerrainCategory*)0x8399a0,
        (LTerrainCategory*)0x839980,
        (LTerrainCategory*)0x839970,
    },
    // Gog
    Categories{
        (LTerrainCategory*)0x837978,
        (LTerrainCategory*)0x837960,
        (LTerrainCategory*)0x837940,
        (LTerrainCategory*)0x837950,
        (LTerrainCategory*)0x837930,
        (LTerrainCategory*)0x837920,
    },
    // Scenario Editor
    Categories{
        (LTerrainCategory*)0x664f30,
        (LTerrainCategory*)0x664f18,
        (LTerrainCategory*)0x664ef8,
        (LTerrainCategory*)0x664f08,
        (LTerrainCategory*)0x664ee8,
        (LTerrainCategory*)0x664ed8,
    },
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::TerrainCategories
