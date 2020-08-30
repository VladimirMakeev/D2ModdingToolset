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

#include "unitbranchcat.h"
#include "version.h"
#include <array>

namespace game::UnitBranchCategories {

// clang-format off
static std::array<Categories, 3> categories = {{
    // Akella
    Categories{
        (LUnitBranch*)0x839fc8,
        (LUnitBranch*)0x839fa8,
        (LUnitBranch*)0x839fd8,
        (LUnitBranch*)0x839f98,
        (LUnitBranch*)0x839f78
    },
    // Russobit
    Categories{},
    // Gog
    Categories{}
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::UnitBranchCategories
