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

#include "subracecat.h"
#include "version.h"
#include <array>

namespace game {
namespace SubraceCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LSubRaceCategory*)0x839b38,
        (LSubRaceCategory*)0x839b18,
        (LSubRaceCategory*)0x839af8,
        (LSubRaceCategory*)0x839bc0,
        (LSubRaceCategory*)0x839b68,
        (LSubRaceCategory*)0x839b48,
        (LSubRaceCategory*)0x839bd0,
        (LSubRaceCategory*)0x839b58,
        (LSubRaceCategory*)0x839b08,
        (LSubRaceCategory*)0x839b28,
        (LSubRaceCategory*)0x839ba8,
        (LSubRaceCategory*)0x839b78,
        (LSubRaceCategory*)0x839ae8,
        (LSubRaceCategory*)0x839b88,
        (LSubRaceCategory*)0x839b98
    },
    // Russobit
    Categories{
        (LSubRaceCategory*)0x839b38,
        (LSubRaceCategory*)0x839b18,
        (LSubRaceCategory*)0x839af8,
        (LSubRaceCategory*)0x839bc0,
        (LSubRaceCategory*)0x839b68,
        (LSubRaceCategory*)0x839b48,
        (LSubRaceCategory*)0x839bd0,
        (LSubRaceCategory*)0x839b58,
        (LSubRaceCategory*)0x839b08,
        (LSubRaceCategory*)0x839b28,
        (LSubRaceCategory*)0x839ba8,
        (LSubRaceCategory*)0x839b78,
        (LSubRaceCategory*)0x839ae8,
        (LSubRaceCategory*)0x839b88,
        (LSubRaceCategory*)0x839b98
    },
    // Gog
    Categories{
        (LSubRaceCategory*)0x837ae8,
        (LSubRaceCategory*)0x837ac8,
        (LSubRaceCategory*)0x837aa8,
        (LSubRaceCategory*)0x837b70,
        (LSubRaceCategory*)0x837b18,
        (LSubRaceCategory*)0x837af8,
        (LSubRaceCategory*)0x837b80,
        (LSubRaceCategory*)0x837b08,
        (LSubRaceCategory*)0x837ab8,
        (LSubRaceCategory*)0x837ad8,
        (LSubRaceCategory*)0x837b58,
        (LSubRaceCategory*)0x837b28,
        (LSubRaceCategory*)0x837a98,
        (LSubRaceCategory*)0x837b38,
        (LSubRaceCategory*)0x837b48
    },
    // Scenario Editor
    Categories{
        (LSubRaceCategory*)0x665268,
        (LSubRaceCategory*)0x665248,
        (LSubRaceCategory*)0x665228,
        (LSubRaceCategory*)0x6652f0,
        (LSubRaceCategory*)0x665298,
        (LSubRaceCategory*)0x665278,
        (LSubRaceCategory*)0x665300,
        (LSubRaceCategory*)0x665288,
        (LSubRaceCategory*)0x665238,
        (LSubRaceCategory*)0x665258,
        (LSubRaceCategory*)0x6652d8,
        (LSubRaceCategory*)0x6652a8,
        (LSubRaceCategory*)0x665218,
        (LSubRaceCategory*)0x6652b8,
        (LSubRaceCategory*)0x6652c8
    }
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6d3634,
    // Russobit
    (const void*)0x6d3634,
    // Gog
    (const void*)0x6d15d4,
    // Scenario Editor
    (const void*)0x5d300c
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

} // namespace SubraceCategories

namespace LSubRaceCategoryTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x58dba4,
        (Api::Init)0x58de08,
        (Api::ReadCategory)0x58de80,
        (Api::InitDone)0x58ddc3,
        (Api::FindCategoryById)0x599025,
    },
    // Russobit
    Api{
        (Api::Constructor)0x58dba4,
        (Api::Init)0x58de08,
        (Api::ReadCategory)0x58de80,
        (Api::InitDone)0x58ddc3,
        (Api::FindCategoryById)0x599025,
    },
    // Gog
    Api{
        (Api::Constructor)0x58ccb9,
        (Api::Init)0x58cf1d,
        (Api::ReadCategory)0x58cf95,
        (Api::InitDone)0x58ced8,
        (Api::FindCategoryById)0x59819a,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x52fd87,
        (Api::Init)0x52ffeb,
        (Api::ReadCategory)0x530063,
        (Api::InitDone)0x52ffa6,
        (Api::FindCategoryById)0x4fbf66,
    }
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6eaaac,
    // Russobit
    (const void*)0x6eaaac,
    // Gog
    (const void*)0x6e8a4c,
    // Scenario Editor
    (const void*)0x5de1bc
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

} // namespace LSubRaceCategoryTableApi

} // namespace game
