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

#include "buildingcat.h"
#include "version.h"
#include <array>

namespace game {
namespace BuildingCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LBuildingCategory*)0x839a30,
        (LBuildingCategory*)0x839a40,
        (LBuildingCategory*)0x839a50,
        (LBuildingCategory*)0x839a20
    },
    // Russobit
    Categories{
        (LBuildingCategory*)0x839a30,
        (LBuildingCategory*)0x839a40,
        (LBuildingCategory*)0x839a50,
        (LBuildingCategory*)0x839a20
    },
    // Gog
    Categories{
        (LBuildingCategory*)0x8379e0,
        (LBuildingCategory*)0x8379f0,
        (LBuildingCategory*)0x837a00,
        (LBuildingCategory*)0x8379d0
    },
    // Scenario Editor
    Categories{
        (LBuildingCategory*)0x665af8,
        (LBuildingCategory*)0x665b08,
        (LBuildingCategory*)0x665b18,
        (LBuildingCategory*)0x665ae8
    }
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6d153c,
    // Russobit
    (const void*)0x6d153c,
    // Gog
    (const void*)0x6cf4dc,
    // Scenario Editor
    (const void*)0x5df5dc
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

} // namespace BuildingCategories

namespace LBuildingCategoryTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x58b4a0,
        (Api::Init)0x58b607,
        (Api::ReadCategory)0x58b67f,
        (Api::InitDone)0x58b5c2,
        (Api::FindCategoryById)0x58c13d,
    },
    // Russobit
    Api{
        (Api::Constructor)0x58b4a0,
        (Api::Init)0x58b607,
        (Api::ReadCategory)0x58b67f,
        (Api::InitDone)0x58b5c2,
        (Api::FindCategoryById)0x58c13d,
    },
    // Gog
    Api{
        (Api::Constructor)0x58a60c,
        (Api::Init)0x58a773,
        (Api::ReadCategory)0x58a7eb,
        (Api::InitDone)0x58a72e,
        (Api::FindCategoryById)0x58b2cb,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x53b0a8,
        (Api::Init)0x53b20f,
        (Api::ReadCategory)0x53b287,
        (Api::InitDone)0x53b1ca,
        (Api::FindCategoryById)0x538d2d,
    }
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6ea71c,
    // Russobit
    (const void*)0x6ea71c,
    // Gog
    (const void*)0x6e86bc,
    // Scenario Editor
    (const void*)0x5df714
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

} // namespace LBuildingCategoryTableApi

} // namespace game
