/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#include "attackclasscat.h"
#include "version.h"
#include <array>

namespace game::AttackClassCategories {

// clang-format off
static std::array<Categories, 3> categories = {{
    // Akella
    Categories{
        (LAttackClass*)0x8394d0,
        (LAttackClass*)0x839590,
        (LAttackClass*)0x839470,
        (LAttackClass*)0x8395c8,
        (LAttackClass*)0x8394e0,
        (LAttackClass*)0x839480,
        (LAttackClass*)0x839510,
        (LAttackClass*)0x8394f0,
        (LAttackClass*)0x839560,
        (LAttackClass*)0x839540,
        (LAttackClass*)0x8395a0,
        (LAttackClass*)0x839490,
        (LAttackClass*)0x839500,
        (LAttackClass*)0x839520,
        (LAttackClass*)0x839550,
        (LAttackClass*)0x8394a0,
        (LAttackClass*)0x839580,
        (LAttackClass*)0x839530,
        (LAttackClass*)0x8395b8,
        (LAttackClass*)0x8394b0,
        (LAttackClass*)0x839570,
        (LAttackClass*)0x8394c0,
        (LAttackClass*)0x8395d8
    },
    // Russobit
    Categories{
        (LAttackClass*)0x8394d0,
        (LAttackClass*)0x839590,
        (LAttackClass*)0x839470,
        (LAttackClass*)0x8395c8,
        (LAttackClass*)0x8394e0,
        (LAttackClass*)0x839480,
        (LAttackClass*)0x839510,
        (LAttackClass*)0x8394f0,
        (LAttackClass*)0x839560,
        (LAttackClass*)0x839540,
        (LAttackClass*)0x8395a0,
        (LAttackClass*)0x839490,
        (LAttackClass*)0x839500,
        (LAttackClass*)0x839520,
        (LAttackClass*)0x839550,
        (LAttackClass*)0x8394a0,
        (LAttackClass*)0x839580,
        (LAttackClass*)0x839530,
        (LAttackClass*)0x8395b8,
        (LAttackClass*)0x8394b0,
        (LAttackClass*)0x839570,
        (LAttackClass*)0x8394c0,
        (LAttackClass*)0x8395d8
    },
    // Gog
    Categories{
        (LAttackClass*)0x837480,
        (LAttackClass*)0x837540,
        (LAttackClass*)0x837420,
        (LAttackClass*)0x837578,
        (LAttackClass*)0x837490,
        (LAttackClass*)0x837430,
        (LAttackClass*)0x8374c0,
        (LAttackClass*)0x8374a0,
        (LAttackClass*)0x837510,
        (LAttackClass*)0x8374f0,
        (LAttackClass*)0x837550,
        (LAttackClass*)0x837440,
        (LAttackClass*)0x8374b0,
        (LAttackClass*)0x8374d0,
        (LAttackClass*)0x837500,
        (LAttackClass*)0x837450,
        (LAttackClass*)0x837530,
        (LAttackClass*)0x8374e0,
        (LAttackClass*)0x837568,
        (LAttackClass*)0x837460,
        (LAttackClass*)0x837520,
        (LAttackClass*)0x837470,
        (LAttackClass*)0x837588
    }
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::AttackClassCategories
