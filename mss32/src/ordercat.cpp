/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#include "ordercat.h"
#include "version.h"
#include <array>

namespace game::OrderCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LOrderCategory*)0x839898,
        (LOrderCategory*)0x8398a8,
        (LOrderCategory*)0x8398b8,
        (LOrderCategory*)0x8398c8,
        (LOrderCategory*)0x8398d8,
        (LOrderCategory*)0x8398e8,
        (LOrderCategory*)0x8398f8,
        (LOrderCategory*)0x839908,
        (LOrderCategory*)0x839918,
        (LOrderCategory*)0x839928,
        (LOrderCategory*)0x839938,
        (LOrderCategory*)0x839948,
        (LOrderCategory*)0x839958,
    },
    // Russobit
    Categories{
        (LOrderCategory*)0x839898,
        (LOrderCategory*)0x8398a8,
        (LOrderCategory*)0x8398b8,
        (LOrderCategory*)0x8398c8,
        (LOrderCategory*)0x8398d8,
        (LOrderCategory*)0x8398e8,
        (LOrderCategory*)0x8398f8,
        (LOrderCategory*)0x839908,
        (LOrderCategory*)0x839918,
        (LOrderCategory*)0x839928,
        (LOrderCategory*)0x839938,
        (LOrderCategory*)0x839948,
        (LOrderCategory*)0x839958,
    },
    // Gog
    Categories{
        (LOrderCategory*)0x837848,
        (LOrderCategory*)0x837858,
        (LOrderCategory*)0x837868,
        (LOrderCategory*)0x837878,
        (LOrderCategory*)0x837888,
        (LOrderCategory*)0x837898,
        (LOrderCategory*)0x8378a8,
        (LOrderCategory*)0x8378b8,
        (LOrderCategory*)0x8378c8,
        (LOrderCategory*)0x8378d8,
        (LOrderCategory*)0x8378e8,
        (LOrderCategory*)0x8378f8,
        (LOrderCategory*)0x837908,
    },
    // Scenario Editor
    Categories{
        (LOrderCategory*)0x6655c0,
        (LOrderCategory*)0x6655d0,
        (LOrderCategory*)0x6655e0,
        (LOrderCategory*)0x6655f0,
        (LOrderCategory*)0x665600,
        (LOrderCategory*)0x665610,
        (LOrderCategory*)0x665620,
        (LOrderCategory*)0x665630,
        (LOrderCategory*)0x665640,
        (LOrderCategory*)0x665650,
        (LOrderCategory*)0x665660,
        (LOrderCategory*)0x665670,
        (LOrderCategory*)0x665680,
    },
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::OrderCategories
