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

#include "listbox.h"
#include "version.h"
#include <array>

namespace game::CListBoxInterfApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::AssignFunctor)0x5c9b66,
        (Api::AssignDisplayTextFunctor)0x5c9982,
        (Api::SetElementsTotal)0x536047,
        (Api::SelectedIndex)0x5838db,
        (Api::SetSelectedIndex)0x536026,
    },
    // Russobit
    Api{
        (Api::AssignFunctor)0x5c9b66,
        (Api::AssignDisplayTextFunctor)0x5c9982,
        (Api::SetElementsTotal)0x536047,
        (Api::SelectedIndex)0x5838db,
        (Api::SetSelectedIndex)0x536026,
    },
    // Gog
    Api{
        (Api::AssignFunctor)0x5c8b34,
        (Api::AssignDisplayTextFunctor)0x5c8950,
        (Api::SetElementsTotal)0x535665,
        (Api::SelectedIndex)0x535361,
        (Api::SetSelectedIndex)0x535644,
    },
    // Scenario Editor
    Api{
        (Api::AssignFunctor)0,
        (Api::AssignDisplayTextFunctor)0x4d1524,
        (Api::SetElementsTotal)0x48d410,
        (Api::SelectedIndex)0x48d127,
        (Api::SetSelectedIndex)0x48d3ef,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CListBoxInterfApi
