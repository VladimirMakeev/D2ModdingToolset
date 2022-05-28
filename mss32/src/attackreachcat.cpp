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

#include "attackreachcat.h"
#include "version.h"
#include <array>

namespace game::AttackReachCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LAttackReach*)0x83a110,
        (LAttackReach*)0x83a120,
        (LAttackReach*)0x83a100
    },
    // Russobit
    Categories{
        (LAttackReach*)0x83a110,
        (LAttackReach*)0x83a120,
        (LAttackReach*)0x83a100
    },
    // Gog
    Categories{
        (LAttackReach*)0x8380c0,
        (LAttackReach*)0x8380d0,
        (LAttackReach*)0x8380b0
    },
    // Scenario Editor
    Categories{
        (LAttackReach*)0x665908,
        (LAttackReach*)0x665918,
        (LAttackReach*)0x6658f8,
    },
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6ceacc,
    // Russobit
    (const void*)0x6ceacc,
    // Gog
    (const void*)0x6cca6c,
    // Scenario Editor
    (const void*)0x5df46c,
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

} // namespace game::AttackReachCategories

namespace game::LAttackReachTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x5926dc,
        (Api::Init)0x59282c,
        (Api::ReadCategory)0x5928a4,
        (Api::InitDone)0x5927e7,
        (Api::FindCategoryById)0x40ad68,
    },
    // Russobit
    Api{
        (Api::Constructor)0x5926dc,
        (Api::Init)0x59282c,
        (Api::ReadCategory)0x5928a4,
        (Api::InitDone)0x5927e7,
        (Api::FindCategoryById)0x40ad68,
    },
    // Gog
    Api{
        (Api::Constructor)0x5917f4,
        (Api::Init)0x591944,
        (Api::ReadCategory)0x5919bc,
        (Api::InitDone)0x5918ff,
        (Api::FindCategoryById)0x40a9f4,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x537abb,
        (Api::Init)0x537c0b,
        (Api::ReadCategory)0x537c83,
        (Api::InitDone)0x537bc6,
        (Api::FindCategoryById)0x54ece4,
    },
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6eb3ec,
    // Russobit
    (const void*)0x6eb3ec,
    // Gog
    (const void*)0x6e938c,
    // Scenario Editor
    (const void*)0x5df474,
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

} // namespace game::LAttackReachTableApi
