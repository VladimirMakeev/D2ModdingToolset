/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#include "spinbuttoninterf.h"
#include "version.h"
#include <array>

namespace game::CSpinButtonInterfApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::SetValues)0x53a661,
        (Api::SetOptions)0x53a5c1,
        (Api::SetSelectedOption)0x53a59c,
        (Api::AssignFunctor)0x5c9bf6,
    },
    // Russobit
    Api{
        (Api::SetValues)0x53a661,
        (Api::SetOptions)0x53a5c1,
        (Api::SetSelectedOption)0x53a59c,
        (Api::AssignFunctor)0x5c9bf6,
    },
    // Gog
    Api{
        (Api::SetValues)0x539d02,
        (Api::SetOptions)0x539c62,
        (Api::SetSelectedOption)0x539c3d,
        (Api::AssignFunctor)0x5c8bc4,
    },
    // Scenario Editor
    Api{
        (Api::SetValues)0,
        (Api::SetOptions)0,
        (Api::SetSelectedOption)0,
        (Api::AssignFunctor)0,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CSpinButtonInterfApi
