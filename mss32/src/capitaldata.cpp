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

#include "capitaldata.h"
#include "version.h"
#include <array>

namespace game::CapitalDataApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Allocate)0x56ad71,
        (Api::GetCapitalData)0x56aea4,
        (Api::CapitalDataPtrSetData)0x40813a,
        (Api::Read)0x61d17b,
        (Api::ReadEntry)0x683318,
    },
    // Russobit
    Api{
        (Api::Allocate)0x56ad71,
        (Api::GetCapitalData)0x56aea4,
        (Api::CapitalDataPtrSetData)0x40813a,
        (Api::Read)0x61d17b,
        (Api::ReadEntry)0x683318,
    },
    // Gog
    Api{
        (Api::Allocate)0x56a421,
        (Api::GetCapitalData)0x56a554,
        (Api::CapitalDataPtrSetData)0x407dc5,
        (Api::Read)0x61bc90,
        (Api::ReadEntry)0x681ca2,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CapitalDataApi
