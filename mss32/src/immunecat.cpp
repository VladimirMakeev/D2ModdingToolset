/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#include "immunecat.h"
#include "version.h"
#include <array>

namespace game::ImmuneCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LImmuneCat*)0x8393a8,
        (LImmuneCat*)0x8393b8,
        (LImmuneCat*)0x8393c8,
    },
    // Russobit
    Categories{
        (LImmuneCat*)0x8393a8,
        (LImmuneCat*)0x8393b8,
        (LImmuneCat*)0x8393c8,
    },
    // Gog
    Categories{
        (LImmuneCat*)0x837358,
        (LImmuneCat*)0x837368,
        (LImmuneCat*)0x837378,
    },
    // Scenario Editor
    Categories{
        (LImmuneCat*)0x665b30,
        (LImmuneCat*)0x665b40,
        (LImmuneCat*)0x665b50,
    },
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6e9cfc,
    // Russobit
    (const void*)0x6e9cfc,
    // Gog
    (const void*)0x6e7c9c,
    // Scenario Editor
    (const void*)0x5df754,
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

} // namespace game::ImmuneCategories

namespace game::LImmuneCatTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x584c37,
        (Api::Init)0x584d87,
        (Api::ReadCategory)0x584dff,
        (Api::InitDone)0x584d42,
        (Api::FindCategoryById)0x5a032c,
    },
    // Russobit
    Api{
        (Api::Constructor)0x584c37,
        (Api::Init)0x584d87,
        (Api::ReadCategory)0x584dff,
        (Api::InitDone)0x584d42,
        (Api::FindCategoryById)0x5a032c,
    },
    // Gog
    Api{
        (Api::Constructor)0x583dea,
        (Api::Init)0x583f3a,
        (Api::ReadCategory)0x583fb2,
        (Api::InitDone)0x583ef5,
        (Api::FindCategoryById)0x59f5c2,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x53b4aa,
        (Api::Init)0x53b5fa,
        (Api::ReadCategory)0x53b672,
        (Api::InitDone)0x53b5b5,
        (Api::FindCategoryById)0x5485a1,
    },
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6e9d04,
    // Russobit
    (const void*)0x6e9d04,
    // Gog
    (const void*)0x6e7ca4,
    // Scenario Editor
    (const void*)0x5df75c,
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

} // namespace game::LImmuneCatTableApi
