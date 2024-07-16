/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#include "nobleactioncat.h"
#include "version.h"
#include <array>

namespace game {

namespace NobleActionCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LNobleActionCat*)0x839ea0,
        (LNobleActionCat*)0x839e90,
        (LNobleActionCat*)0x839eb0,
        (LNobleActionCat*)0x839f10,
        (LNobleActionCat*)0x839f30,
        (LNobleActionCat*)0x839f48,
        (LNobleActionCat*)0x839ee0,
        (LNobleActionCat*)0x839ec0,
        (LNobleActionCat*)0x839ed0,
        (LNobleActionCat*)0x839f58,
        (LNobleActionCat*)0x839f20,
        (LNobleActionCat*)0x839f00,
        (LNobleActionCat*)0x839ef0,
        (LNobleActionCat*)0x839e80,
    },
    // Russobit
    Categories{
        (LNobleActionCat*)0x839ea0,
        (LNobleActionCat*)0x839e90,
        (LNobleActionCat*)0x839eb0,
        (LNobleActionCat*)0x839f10,
        (LNobleActionCat*)0x839f30,
        (LNobleActionCat*)0x839f48,
        (LNobleActionCat*)0x839ee0,
        (LNobleActionCat*)0x839ec0,
        (LNobleActionCat*)0x839ed0,
        (LNobleActionCat*)0x839f58,
        (LNobleActionCat*)0x839f20,
        (LNobleActionCat*)0x839f00,
        (LNobleActionCat*)0x839ef0,
        (LNobleActionCat*)0x839e80,
    },
    // Gog
    Categories{
        (LNobleActionCat*)0x837e50,
        (LNobleActionCat*)0x837e40,
        (LNobleActionCat*)0x837e60,
        (LNobleActionCat*)0x837ec0,
        (LNobleActionCat*)0x837ee0,
        (LNobleActionCat*)0x837ef8,
        (LNobleActionCat*)0x837e90,
        (LNobleActionCat*)0x837e70,
        (LNobleActionCat*)0x837e80,
        (LNobleActionCat*)0x837f08,
        (LNobleActionCat*)0x837ed0,
        (LNobleActionCat*)0x837eb0,
        (LNobleActionCat*)0x837ea0,
        (LNobleActionCat*)0x837e30,
    },
    // Scenario Editor
    Categories{
        (LNobleActionCat*)0x665ce0,
        (LNobleActionCat*)0x665cd0,
        (LNobleActionCat*)0x665cf0,
        (LNobleActionCat*)0x665d50,
        (LNobleActionCat*)0x665d70,
        (LNobleActionCat*)0x665d88,
        (LNobleActionCat*)0x665d20,
        (LNobleActionCat*)0x665d00,
        (LNobleActionCat*)0x665d10,
        (LNobleActionCat*)0x665d98,
        (LNobleActionCat*)0x665d60,
        (LNobleActionCat*)0x665d40,
        (LNobleActionCat*)0x665d30,
        (LNobleActionCat*)0x665cc0,
    }
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace NobleActionCategories

namespace LNobleActionCatTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x58f64f,
        (Api::Init)0x58f89c,
        (Api::ReadCategory)0x58f914,
        (Api::InitDone)0x58f857,
        (Api::FindCategoryById)0,
    },
    // Russobit
    Api{
        (Api::Constructor)0x58f64f,
        (Api::Init)0x58f89c,
        (Api::ReadCategory)0x58f914,
        (Api::InitDone)0x58f857,
        (Api::FindCategoryById)0,
    },
    // Gog
    Api{
        (Api::Constructor)0x58e764,
        (Api::Init)0x58e9b1,
        (Api::ReadCategory)0x58ea29,
        (Api::InitDone)0x58e96c,
        (Api::FindCategoryById)0,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x54047a,
        (Api::Init)0x5406c7,
        (Api::ReadCategory)0x54073f,
        (Api::InitDone)0x540682,
        (Api::FindCategoryById)0,
    }
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6eae34,
    // Russobit
    (const void*)0x6eae34,
    // Gog
    (const void*)0x6e8dd4,
    // Scenario Editor
    (const void*)0x5dfd4c,
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

} // namespace LNobleActionCatTableApi

} // namespace game
