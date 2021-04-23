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

#include "globaldata.h"
#include "version.h"
#include <array>

namespace game::GlobalDataApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::GetGlobalData)0x58004f,
        (Api::FindById)0x583c35,
        (Api::FindItemById)0x58452a,
        (Api::FindDynUpgradeById)0x598644,
    },
    // Russobit
    Api{
        (Api::GetGlobalData)0x58004f,
        (Api::FindById)0x583c35,
        (Api::FindItemById)0x58452a,
        (Api::FindDynUpgradeById)0x598644,
    },
    // Gog
    Api{
        (Api::GetGlobalData)0x57f707,
        (Api::FindById)0x592f25,
        (Api::FindItemById)0x5836dd,
        (Api::FindDynUpgradeById)0x597781,
    },
    // Scenario Editor
    Api{
        (Api::GetGlobalData)0x5286ce,
        (Api::FindById)0x538beb,
        (Api::FindItemById)0x52d10b,
        (Api::FindDynUpgradeById)0x542c85,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::GlobalDataApi
