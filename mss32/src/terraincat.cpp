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

namespace game {
namespace TerrainCategories {

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

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6cf9d4,
    // Russobit
    (const void*)0x6cf9d4,
    // Gog
    (const void*)0x6cd974,
    // Scenario Editor
    (const void*)0x5ca8e4
}};

// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

const void* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace TerrainCategories

namespace LTerrainCategoryTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x58a72a,
        (Api::Init)0x58a8bf,
        (Api::ReadCategory)0x58a937,
        (Api::InitDone)0x58a87a,
        (Api::FindCategoryById)0x4791c3,
    },
    // Russobit
    Api{
        (Api::Constructor)0x58a72a,
        (Api::Init)0x58a8bf,
        (Api::ReadCategory)0x58a937,
        (Api::InitDone)0x58a87a,
        (Api::FindCategoryById)0x4791c3,
    },
    // Gog
    Api{
        (Api::Constructor)0x589899,
        (Api::Init)0x589a2e,
        (Api::ReadCategory)0x589aa6,
        (Api::InitDone)0x5899e9,
        (Api::FindCategoryById)0x478d8f,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x52aaa4,
        (Api::Init)0x52ac39,
        (Api::ReadCategory)0x52acb1,
        (Api::InitDone)0x52abf4,
        (Api::FindCategoryById)0x4e443a,
    }
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6ea63c,
    // Russobit
    (const void*)0x6ea63c,
    // Gog
    (const void*)0x6e85dc,
    // Scenario Editor
    (const void*)0x5ddb9c
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

const void* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace LTerrainCategoryTableApi

} // namespace game
