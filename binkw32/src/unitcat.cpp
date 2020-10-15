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

#include "unitcat.h"
#include "version.h"
#include <array>

namespace game::UnitCategories {

// clang-format off
static std::array<Categories, 3> categories = {{
    // Akella
    Categories{
        (LUnitCategory*)0x83a1a0,
        (LUnitCategory*)0x83a1e0,
        (LUnitCategory*)0x83a1d0,
        (LUnitCategory*)0x83a1b0,
        (LUnitCategory*)0x83a1c0,
        (LUnitCategory*)0x83a1f0
    },
    // Russobit
    Categories{
        (LUnitCategory*)0x83a1a0,
        (LUnitCategory*)0x83a1e0,
        (LUnitCategory*)0x83a1d0,
        (LUnitCategory*)0x83a1b0,
        (LUnitCategory*)0x83a1c0,
        (LUnitCategory*)0x83a1f0
    },
    // Gog
    Categories{
        (LUnitCategory*)0x838150,
        (LUnitCategory*)0x838190,
        (LUnitCategory*)0x838180,
        (LUnitCategory*)0x838160,
        (LUnitCategory*)0x838170,
        (LUnitCategory*)0x8381a0
    }
}};

static std::array<const void*, 3> vftables = {{
    // Akella
    (const void*)0x6ea5cc,
    // Russobit
    (const void*)0x6ea5cc,
    // Gog
    (const void*)0x6e856c
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

} // namespace game::UnitCategories
