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
static std::array<Categories, 3> categories = {{
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
    }
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::AttackSourceCategories
