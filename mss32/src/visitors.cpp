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

#include "visitors.h"
#include "version.h"
#include <array>

namespace game::VisitorApi {

// clang-format off
std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::ChangeUnitHp)0x5e88f4,
        (Api::ForceUnitMax)0x5e972a,
        (Api::AddUnitToGroup)0x5e8bf8
    },
    // Russobit
    Api{
        (Api::ChangeUnitHp)0x5e88f4,
        (Api::ForceUnitMax)0x5e972a,
        (Api::AddUnitToGroup)0x5e972a
    },
    // Gog
    Api{
        (Api::ChangeUnitHp)0x5e75f3,
        (Api::ForceUnitMax)0x5e8429,
        (Api::AddUnitToGroup)0x5e78f7
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::VisitorApi
