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

#include "midgardmapblock.h"
#include "version.h"
#include <array>

namespace game::CMidgardMapBlockApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Allocate)0x6851d3,
        (Api::Constructor)0x5f43c1,
        (Api::CountTerrainCoverage)0x5f46fc,
        (Api::SetTerrain)0x5f47dd,
        (Api::GetTerrain)0x5f444b,
        (Api::SetGround)0x5f484f,
        (Api::GetGround)0x5f44d9,
    },
    // Russobit
    Api{
        (Api::Allocate)0x6851d3,
        (Api::Constructor)0x5f43c1,
        (Api::CountTerrainCoverage)0x5f46fc,
        (Api::SetTerrain)0x5f47dd,
        (Api::GetTerrain)0x5f444b,
        (Api::SetGround)0x5f484f,
        (Api::GetGround)0x5f44d9,
    },
    // Gog
    Api{
        (Api::Allocate)0x683b5d,
        (Api::Constructor)0x5f308e,
        (Api::CountTerrainCoverage)0x5f33c9,
        (Api::SetTerrain)0x5f34aa,
        (Api::GetTerrain)0x5f3118,
        (Api::SetGround)0x5f351c,
        (Api::GetGround)0x5f31a6,
    },
    // Scenario Editor
    Api{
        (Api::Allocate)0x4da668,
        (Api::Constructor)0x5022ee,
        (Api::CountTerrainCoverage)0x502642,
        (Api::SetTerrain)0x502723,
        (Api::GetTerrain)0x502408,
        (Api::SetGround)0x502795,
        (Api::GetGround)0x502496,
    },
}};

static std::array<IMidScenarioObjectVftable*, 4> vftables = {{
    // Akella
    (IMidScenarioObjectVftable*)0x6f0dbc,
    // Russobit
    (IMidScenarioObjectVftable*)0x6f0dbc,
    // Gog
    (IMidScenarioObjectVftable*)0x6eed5c,
    // Scenario Editor
    (IMidScenarioObjectVftable*)0x5db45c,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

IMidScenarioObjectVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMidgardMapBlockApi
