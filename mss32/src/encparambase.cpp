/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#include "encparambase.h"
#include "version.h"
#include <array>

namespace game::CEncParamBaseApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::AddUnitBattleInfo)0x63b1a1,
        (Api::GetData)0x57437a,
        (Api::SetData)0x574342,
    },
    // Russobit
    Api{
        (Api::AddUnitBattleInfo)0x63b1a1,
        (Api::GetData)0x57437a,
        (Api::SetData)0x574342,
    },
    // Gog
    Api{
        (Api::AddUnitBattleInfo)0x639be1,
        (Api::GetData)0x5739cc,
        (Api::SetData)0x573994,
    },
    // Scenario Editor
    Api{
        (Api::AddUnitBattleInfo)nullptr,
        (Api::GetData)0x4c4ddb,
        (Api::SetData)nullptr,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CEncParamBaseApi
