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

#include "difficultylevel.h"
#include "version.h"
#include <array>

namespace game::DifficultyLevelCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LDifficultyLevel*)0x8399d8,
        (LDifficultyLevel*)0x839a08,
        (LDifficultyLevel*)0x8399f8,
        (LDifficultyLevel*)0x8399e8
    },
    // Russobit
    Categories{
        (LDifficultyLevel*)0x8399d8,
        (LDifficultyLevel*)0x839a08,
        (LDifficultyLevel*)0x8399f8,
        (LDifficultyLevel*)0x8399e8
    },
    // Gog
    Categories{
        (LDifficultyLevel*)0x837988,
        (LDifficultyLevel*)0x8379b8,
        (LDifficultyLevel*)0x8379a8,
        (LDifficultyLevel*)0x837998
    },
    // Scenario Editor
    Categories{
        (LDifficultyLevel*)0x6651d0,
        (LDifficultyLevel*)0x665200,
        (LDifficultyLevel*)0x6651f0,
        (LDifficultyLevel*)0x6651e0
    }
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::DifficultyLevelCategories
