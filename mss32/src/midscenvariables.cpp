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

#include "midscenvariables.h"
#include "version.h"
#include <array>

namespace game::CMidScenVariablesApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Advance)0x560f17,
        (Api::GetData)0x5fe73d,
    },
    // Russobit
    Api{
        (Api::Advance)0x560f17,
        (Api::GetData)0x5fe73d,
    },
    // Gog
    Api{
        (Api::Advance)0x5606b4,
        (Api::GetData)0x5fd39d,
    },
    // Scenario Editor
    Api{
        (Api::Advance)0x4c47b2,
        (Api::GetData)0x4e677a,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMidScenVariablesApi
