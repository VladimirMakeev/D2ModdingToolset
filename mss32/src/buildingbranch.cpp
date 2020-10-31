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

#include "buildingbranch.h"
#include "version.h"
#include <array>

namespace game::CBuildingBranchApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x4aaa69,
        (Api::InitData)0x4aaee5,
        (Api::InitData2)0x4ab326,
        (Api::InitData3)0x4ab246,
        (Api::AddSideshowUnitBuilding)0x4aae31,
        (Api::AddUnitBuilding)0x4aad34,
        (Api::AddBuilding)0x4aadbf
    },
    // Russobit
    Api{
        (Api::Constructor)0x4aaa69,
        (Api::InitData)0x4aaee5,
        (Api::InitData2)0x4ab326,
        (Api::InitData3)0x4ab246,
        (Api::AddSideshowUnitBuilding)0x4aae31,
        (Api::AddUnitBuilding)0x4aad34,
        (Api::AddBuilding)0x4aadbf
    },
    // Gog
    Api{
        (Api::Constructor)0x4aa283,
        (Api::InitData)0x4aa6ff,
        (Api::InitData2)0x4aab40,
        (Api::InitData3)0x4aaa60,
        (Api::AddSideshowUnitBuilding)0x4aa64b,
        (Api::AddUnitBuilding)0x4aa54e,
        (Api::AddBuilding)0x4aa5d9
    }
}};

static std::array<const void*, 3> vftables = {{
    // Akella
    (const void*)0x6d8e64,
    // Russobit
    (const void*)0x6d8e64,
    // Gog
    (const void*)0x6d6e04
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

} // namespace game::CBuildingBranchApi
