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

#include "midunitgroup.h"
#include "version.h"
#include <array>

namespace game::CMidUnitGroupApi {

// clang-format off
std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::GetUnitId)0x5f57f1,
        (Api::GetUnitPosition)0x5f5897,
        (Api::GetUnitIdByPosition)0x5f586f,
        (Api::AddUnitIdsAvailableForSummons)0x628a02,
        (Api::UnknownFunction)0x631c50,
        (Api::GetUnitsCount)0x5f5779,
    },
    // Russobit
    Api{
        (Api::GetUnitId)0x5f57f1,
        (Api::GetUnitPosition)0x5f5897,
        (Api::GetUnitIdByPosition)0x5f586f,
        (Api::AddUnitIdsAvailableForSummons)0x628a02,
        (Api::UnknownFunction)0x631c50,
        (Api::GetUnitsCount)0x5f5779,
    },
    // Gog
    Api{
        (Api::GetUnitId)0x5f44b9,
        (Api::GetUnitPosition)0x5f455f,
        (Api::GetUnitIdByPosition)0x5f4537,
        (Api::AddUnitIdsAvailableForSummons)0x627542,
        (Api::UnknownFunction)0x630690,
        (Api::GetUnitsCount)0x5f4441,
    },
    // Scenario Editor
    Api{
        (Api::GetUnitId)0x4f03e8,
        (Api::GetUnitPosition)0x4f0444,
        (Api::GetUnitIdByPosition)0x4f041c,
        (Api::AddUnitIdsAvailableForSummons)nullptr,
        (Api::UnknownFunction)nullptr,
        (Api::GetUnitsCount)0x4f0370,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMidUnitGroupApi
