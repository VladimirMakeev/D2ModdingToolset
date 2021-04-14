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

#include "attacksourcecat.h"
#include "version.h"
#include <array>

namespace game::AttackSourceCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LAttackSource*)0x839420,
        (LAttackSource*)0x839440,
        (LAttackSource*)0x839458,
        (LAttackSource*)0x839430,
        (LAttackSource*)0x839410,
        (LAttackSource*)0x8393e0,
        (LAttackSource*)0x839400,
        (LAttackSource*)0x8393f0
    },
    // Russobit
    Categories{
        (LAttackSource*)0x839420,
        (LAttackSource*)0x839440,
        (LAttackSource*)0x839458,
        (LAttackSource*)0x839430,
        (LAttackSource*)0x839410,
        (LAttackSource*)0x8393e0,
        (LAttackSource*)0x839400,
        (LAttackSource*)0x8393f0
    },
    // Gog
    Categories{
        (LAttackSource*)0x8373d0,
        (LAttackSource*)0x8373f0,
        (LAttackSource*)0x837408,
        (LAttackSource*)0x8373e0,
        (LAttackSource*)0x8373c0,
        (LAttackSource*)0x837390,
        (LAttackSource*)0x8373b0,
        (LAttackSource*)0x8373a0
    },
    // Scenario Editor
    Categories{
        (LAttackSource*)0x665ba8,
        (LAttackSource*)0x665bc8,
        (LAttackSource*)0x665be0,
        (LAttackSource*)0x665bb8,
        (LAttackSource*)0x665b98,
        (LAttackSource*)0x665b68,
        (LAttackSource*)0x665b88,
        (LAttackSource*)0x665b78,
    },
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6cead4,
    // Russobit
    (const void*)0x6cead4,
    // Gog
    (const void*)0x6cca74,
    // Scenario Editor
    (const void*)0x5da8cc,
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

} // namespace game::AttackSourceCategories

namespace game::LAttackSourceTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x585143,
        (Api::Init)0x585306,
        (Api::ReadCategory)0x58537e,
        (Api::InitDone)0x5852c1,
        (Api::FindCategoryById)0x40ad09,
    },
    // Russobit
    Api{
        (Api::Constructor)0x585143,
        (Api::Init)0x585306,
        (Api::ReadCategory)0x58537e,
        (Api::InitDone)0x5852c1,
        (Api::FindCategoryById)0x40ad09,
    },
    // Gog
    Api{
        (Api::Constructor)0x5842f6,
        (Api::Init)0x5844b9,
        (Api::ReadCategory)0x584531,
        (Api::InitDone)0x584474,
        (Api::FindCategoryById)0x40a995,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x53bf3a,
        (Api::Init)0x53c0fd,
        (Api::ReadCategory)0x53c175,
        (Api::InitDone)0x53c0b8,
        (Api::FindCategoryById)0x4f2414,
    },
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6e9d5c,
    // Russobit
    (const void*)0x6e9d5c,
    // Gog
    (const void*)0x6e7cfc,
    // Scenario Editor
    (const void*)0x5df7dc,
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

} // namespace game::LAttackSourceTableApi
