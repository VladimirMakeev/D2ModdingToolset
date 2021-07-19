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

#include "uimanager.h"
#include "version.h"
#include <array>

namespace game::CUIManagerApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Get)0x561afc,
        (Api::CreateUnknownData)0x561f57,
        (Api::UnknownDataCopy)0x562081,
        (Api::UnknownDataDestructor)0x562050,
        (Api::GetMousePosition)0x561c4f,
    },
    // Russobit
    Api{
        (Api::Get)0x561afc,
        (Api::CreateUnknownData)0x561f57,
        (Api::UnknownDataCopy)0x562081,
        (Api::UnknownDataDestructor)0x562050,
        (Api::GetMousePosition)0x561c4f,
    },
    // Gog
    Api{
        (Api::Get)0x561299,
        (Api::CreateUnknownData)0x5616f4,
        (Api::UnknownDataCopy)0x56181e,
        (Api::UnknownDataDestructor)0x5617ed,
        (Api::GetMousePosition)0x5613ec,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CUIManagerApi
