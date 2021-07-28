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

#include "radiobuttoninterf.h"
#include "version.h"
#include <array>

namespace game::CRadioButtonInterfApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::SetEnabled)0,
        (Api::SetButtonEnabled)0,
        (Api::SetCheckedButton)0,
    },
    // Russobit
    Api{
        (Api::SetEnabled)0,
        (Api::SetButtonEnabled)0,
        (Api::SetCheckedButton)0,
    },
    // Gog
    Api{
        (Api::SetEnabled)0,
        (Api::SetButtonEnabled)0,
        (Api::SetCheckedButton)0,
    },
    // Scenario Editor
    Api{
        (Api::SetEnabled)0x48ee1e,
        (Api::SetButtonEnabled)0x48ee30,
        (Api::SetCheckedButton)0x48ede5,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CRadioButtonInterfApi
