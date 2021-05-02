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

#include "dialoginterf.h"
#include "version.h"
#include <array>

namespace game::CDialogInterfApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::FindButton)0x50baaf,
        (Api::FindListBox)0x50bacf,
        (Api::FindTextBox)0x50bb0f,
        (Api::FindControl)0x50c206,
        (Api::FindPicture)0x50badf,
    },
    // Russobit
    Api{
        (Api::FindButton)0x50baaf,
        (Api::FindListBox)0x50bacf,
        (Api::FindTextBox)0x50bb0f,
        (Api::FindControl)0x50c206,
        (Api::FindPicture)0x50badf,
    },
    // Gog
    Api{
        (Api::FindButton)0x50afb3,
        (Api::FindListBox)0x50afd3,
        (Api::FindTextBox)0x50b013,
        (Api::FindControl)0x50b70a,
        (Api::FindPicture)0x50afe3,
    },
    // Scenario Editor
    Api{
        (Api::FindButton)nullptr,
        (Api::FindListBox)nullptr,
        (Api::FindTextBox)0x4a3571,
        (Api::FindControl)nullptr,
        (Api::FindPicture)nullptr,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CDialogInterfApi
