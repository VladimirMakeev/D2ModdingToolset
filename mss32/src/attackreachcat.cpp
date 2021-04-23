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
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::AttackReachCategories
