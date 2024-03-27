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

#include "resourcetype.h"
#include "version.h"
#include <array>

namespace game::ResourceTypes {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LResourceType*)0x83a208,
        (LResourceType*)0x83a248,
        (LResourceType*)0x83a228,
        (LResourceType*)0x83a218,
        (LResourceType*)0x83a258,
        (LResourceType*)0x83a238,
    },
    // Russobit
    Categories{
        (LResourceType*)0x83a208,
        (LResourceType*)0x83a248,
        (LResourceType*)0x83a228,
        (LResourceType*)0x83a218,
        (LResourceType*)0x83a258,
        (LResourceType*)0x83a238,
    },
    // Gog
    Categories{
        (LResourceType*)0x8381b8,
        (LResourceType*)0x8381f8,
        (LResourceType*)0x8381d8,
        (LResourceType*)0x8381c8,
        (LResourceType*)0x838208,
        (LResourceType*)0x8381e8,
    },
    // Scenario Editor
    Categories{
        (LResourceType*)0x664f40,
        (LResourceType*)0x664f80,
        (LResourceType*)0x664f60,
        (LResourceType*)0x664f50,
        (LResourceType*)0x664f90,
        (LResourceType*)0x664f70,
    }
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::ResourceTypes
