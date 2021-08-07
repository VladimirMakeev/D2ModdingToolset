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

#include "eventconditioncat.h"
#include "version.h"
#include <array>

namespace game {

namespace EventCondCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LEventCondCategory*)0x839e20,
        (LEventCondCategory*)0x839e50,
        (LEventCondCategory*)0x839d90,
        (LEventCondCategory*)0x839e10,
        (LEventCondCategory*)0x839df0,
        (LEventCondCategory*)0x839e60,
        (LEventCondCategory*)0x839da0,
        (LEventCondCategory*)0x839d70,
        (LEventCondCategory*)0x839dd0,
        (LEventCondCategory*)0x839e70,
        (LEventCondCategory*)0x839e40,
        (LEventCondCategory*)0x839de0,
        (LEventCondCategory*)0x839e30,
        (LEventCondCategory*)0x839e00,
        (LEventCondCategory*)0x839db0,
        (LEventCondCategory*)0x839d80,
        (LEventCondCategory*)0x839dc0,
    },
    // Russobit
    Categories{
        (LEventCondCategory*)0x839e20,
        (LEventCondCategory*)0x839e50,
        (LEventCondCategory*)0x839d90,
        (LEventCondCategory*)0x839e10,
        (LEventCondCategory*)0x839df0,
        (LEventCondCategory*)0x839e60,
        (LEventCondCategory*)0x839da0,
        (LEventCondCategory*)0x839d70,
        (LEventCondCategory*)0x839dd0,
        (LEventCondCategory*)0x839e70,
        (LEventCondCategory*)0x839e40,
        (LEventCondCategory*)0x839de0,
        (LEventCondCategory*)0x839e30,
        (LEventCondCategory*)0x839e00,
        (LEventCondCategory*)0x839db0,
        (LEventCondCategory*)0x839d80,
        (LEventCondCategory*)0x839dc0,
    },
    // Gog
    Categories{
        (LEventCondCategory*)0x837dd0,
        (LEventCondCategory*)0x837e00,
        (LEventCondCategory*)0x837d40,
        (LEventCondCategory*)0x837dc0,
        (LEventCondCategory*)0x837da0,
        (LEventCondCategory*)0x837e10,
        (LEventCondCategory*)0x837d50,
        (LEventCondCategory*)0x837d20,
        (LEventCondCategory*)0x837d80,
        (LEventCondCategory*)0x837e20,
        (LEventCondCategory*)0x837df0,
        (LEventCondCategory*)0x837d90,
        (LEventCondCategory*)0x837de0,
        (LEventCondCategory*)0x837db0,
        (LEventCondCategory*)0x837d60,
        (LEventCondCategory*)0x837d30,
        (LEventCondCategory*)0x837d70,
    },
    // Scenario Editor
    Categories{
        (LEventCondCategory*)0x6653d0,
        (LEventCondCategory*)0x665400,
        (LEventCondCategory*)0x665340,
        (LEventCondCategory*)0x6653c0,
        (LEventCondCategory*)0x6653a0,
        (LEventCondCategory*)0x665410,
        (LEventCondCategory*)0x665350,
        (LEventCondCategory*)0x665320,
        (LEventCondCategory*)0x665380,
        (LEventCondCategory*)0x665420,
        (LEventCondCategory*)0x6653f0,
        (LEventCondCategory*)0x665390,
        (LEventCondCategory*)0x6653e0,
        (LEventCondCategory*)0x6653b0,
        (LEventCondCategory*)0x665360,
        (LEventCondCategory*)0x665330,
        (LEventCondCategory*)0x665370,
    },
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6ead4c,
    // Russobit
    (const void*)0x6ead4c,
    // Gog
    (const void*)0x6e8cec,
    // Scenario Editor
    (const void*)0x5cd1c4,
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

} // namespace EventCondCategories

namespace LEventCondCategoryTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x58ee9f,
        (Api::Init)0x58f131,
        (Api::ReadCategory)0x58f1a9,
        (Api::InitDone)0x58f0ec,
        (Api::FindCategoryById)nullptr,
    },
    // Russobit
    Api{
        (Api::Constructor)0x58ee9f,
        (Api::Init)0x58f131,
        (Api::ReadCategory)0x58f1a9,
        (Api::InitDone)0x58f0ec,
        (Api::FindCategoryById)nullptr,
    },
    // Gog
    Api{
        (Api::Constructor)0x58dfb4,
        (Api::Init)0x58e246,
        (Api::ReadCategory)0x58e2be,
        (Api::InitDone)0x58e201,
        (Api::FindCategoryById)nullptr,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x5305c9,
        (Api::Init)0x53085b,
        (Api::ReadCategory)0x5308d3,
        (Api::InitDone)0x530816,
        (Api::FindCategoryById)nullptr,
    },
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6ead54,
    // Russobit
    (const void*)0x6ead54,
    // Gog
    (const void*)0x6e8cf4,
    // Scenario Editor
    (const void*)0x5de2ec,
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

} // namespace LEventCondCategoryTableApi

} // namespace game
