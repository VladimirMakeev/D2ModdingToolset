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

#include "battlemsgdata.h"
#include "version.h"
#include <array>

namespace game::BattleMsgDataApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::GetUnitStatus)0x623b8c,
        (Api::SetUnitStatus)0x623c11,
        (Api::GetUnitIntValue)0x62332c,
        (Api::GetUnitIntValue)0x623391,
        (Api::SetUnitIntValue)0x6232d7,
        (Api::SetUnitInt16Value)0x62423b,
        (Api::CheckUnitDeath)0x65cf69,
        (Api::SetInt8Value)0x6247d9
    },
    // Russobit
    Api{
        (Api::GetUnitStatus)0x623b8c,
        (Api::SetUnitStatus)0x623c11,
        (Api::GetUnitIntValue)0x62332c,
        (Api::GetUnitIntValue)0x623391,
        (Api::SetUnitIntValue)0x6232d7,
        (Api::SetUnitInt16Value)0x62423b,
        (Api::CheckUnitDeath)0x65cf69,
        (Api::SetInt8Value)0x6247d9
    },
    // Gog
    Api{
        (Api::GetUnitStatus)0x62271c,
        (Api::SetUnitStatus)0x6227a1,
        (Api::GetUnitIntValue)0x621ebc,
        (Api::GetUnitIntValue)0x621f21,
        (Api::SetUnitIntValue)0x621e67,
        (Api::SetUnitInt16Value)0x622dcb,
        (Api::CheckUnitDeath)0x65b9e9,
        (Api::SetInt8Value)0x623369
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::BattleMsgDataApi
