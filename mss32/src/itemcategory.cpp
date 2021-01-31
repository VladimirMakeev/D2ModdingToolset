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

#include "itemcategory.h"
#include "version.h"
#include <array>

namespace game::ItemCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LItemCategory*)0x8396e0,
        (LItemCategory*)0x8396d0,
        (LItemCategory*)0x8396f0,
        (LItemCategory*)0x8396c0,
        (LItemCategory*)0x839778,
        (LItemCategory*)0x839740,
        (LItemCategory*)0x8396b0,
        (LItemCategory*)0x839750,
        (LItemCategory*)0x839710,
        (LItemCategory*)0x839720,
        (LItemCategory*)0x839760,
        (LItemCategory*)0x839788,
        (LItemCategory*)0x839700,
        (LItemCategory*)0x839730,
        (LItemCategory*)0x8396a0
    },
    // Russobit
    Categories{
        (LItemCategory*)0x8396e0,
        (LItemCategory*)0x8396d0,
        (LItemCategory*)0x8396f0,
        (LItemCategory*)0x8396c0,
        (LItemCategory*)0x839778,
        (LItemCategory*)0x839740,
        (LItemCategory*)0x8396b0,
        (LItemCategory*)0x839750,
        (LItemCategory*)0x839710,
        (LItemCategory*)0x839720,
        (LItemCategory*)0x839760,
        (LItemCategory*)0x839788,
        (LItemCategory*)0x839700,
        (LItemCategory*)0x839730,
        (LItemCategory*)0x8396a0
    },
    // Gog
    Categories{
        (LItemCategory*)0x837690,
        (LItemCategory*)0x837680,
        (LItemCategory*)0x8376a0,
        (LItemCategory*)0x837670,
        (LItemCategory*)0x837728,
        (LItemCategory*)0x8376f0,
        (LItemCategory*)0x837660,
        (LItemCategory*)0x837700,
        (LItemCategory*)0x8376c0,
        (LItemCategory*)0x8376d0,
        (LItemCategory*)0x837710,
        (LItemCategory*)0x837738,
        (LItemCategory*)0x8376b0,
        (LItemCategory*)0x8376e0,
        (LItemCategory*)0x837650
    },
    // Scenario Editor
    Categories{
        (LItemCategory*)0x665088,
        (LItemCategory*)0x665078,
        (LItemCategory*)0x665098,
        (LItemCategory*)0x665068,
        (LItemCategory*)0x665120,
        (LItemCategory*)0x6650e8,
        (LItemCategory*)0x665058,
        (LItemCategory*)0x6650f8,
        (LItemCategory*)0x6650b8,
        (LItemCategory*)0x6650c8,
        (LItemCategory*)0x665108,
        (LItemCategory*)0x665130,
        (LItemCategory*)0x6650a8,
        (LItemCategory*)0x6650d8,
        (LItemCategory*)0x665048
    }
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::ItemCategories
