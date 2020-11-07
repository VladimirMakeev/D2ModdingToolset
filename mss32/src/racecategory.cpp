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

#include "racecategory.h"
#include "version.h"
#include <array>

namespace game {
namespace RaceCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LRaceCategory*)0x839310,
        (LRaceCategory*)0x839330,
        (LRaceCategory*)0x839320,
        (LRaceCategory*)0x8392e0,
        (LRaceCategory*)0x839300,
        (LRaceCategory*)0x8392f0
    },
    // Russobit
    Categories{
        (LRaceCategory*)0x839310,
        (LRaceCategory*)0x839330,
        (LRaceCategory*)0x839320,
        (LRaceCategory*)0x8392e0,
        (LRaceCategory*)0x839300,
        (LRaceCategory*)0x8392f0
    },
    // Gog
    Categories{
        (LRaceCategory*)0x8372c0,
        (LRaceCategory*)0x8372e0,
        (LRaceCategory*)0x8372d0,
        (LRaceCategory*)0x837290,
        (LRaceCategory*)0x8372b0,
        (LRaceCategory*)0x8372a0
    },
    // Scenario Editor
    Categories{
        (LRaceCategory*)0x665010,
        (LRaceCategory*)0x665030,
        (LRaceCategory*)0x665020,
        (LRaceCategory*)0x664fe0,
        (LRaceCategory*)0x665000,
        (LRaceCategory*)0x664ff0
    }
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6ceae4,
    // Russobit
    (const void*)0x6ceae4,
    // Gog
    (const void*)0x6cca84,
    // Scenario Editor
    (const void*)0x5ca75c
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

} // namespace RaceCategories

namespace LRaceCategoryTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x57f4e1,
        (Api::Init)0x57f676,
        (Api::ReadCategory)0x57f6ee,
        (Api::InitDone)0x57f631
    },
    // Russobit
    Api{
        (Api::Constructor)0x57f4e1,
        (Api::Init)0x57f676,
        (Api::ReadCategory)0x57f6ee,
        (Api::InitDone)0x57f631
    },
    // Gog
    Api{
        (Api::Constructor)0x57eb99,
        (Api::Init)0x57e2de,
        (Api::ReadCategory)0x57eda6,
        (Api::InitDone)0x57ece9
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x52c128,
        (Api::Init)0x52c2bd,
        (Api::ReadCategory)0x52c335,
        (Api::InitDone)0x52c278
    }
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6e9294,
    // Russobit
    (const void*)0x6e9294,
    // Gog
    (const void*)0x6e7234,
    // Scenario Editor
    (const void*)0x5ddcd4
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

} // namespace LRaceCategoryTableApi

} // namespace game
