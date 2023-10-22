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

#include "middiplomacy.h"
#include "version.h"
#include <array>

namespace game::CMidDiplomacyApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::GetRelation)0x5f8bfc,
        (Api::GetRelation)0x5f8c79,
        (Api::GetBool)0x5f8cb0,
        (Api::GetAllianceTurn)0x5f8cf4,
        (Api::GetBool)0x5f8d2b,
        (Api::GetBool)0x5f8cd2,
        (Api::CheckRelationType)0x5f9101,
        (Api::CheckRelationType)0x5f912d,
        (Api::CheckRelationType)0x5f917d,
    },
    // Russobit
    Api{
        (Api::GetRelation)0x5f8bfc,
        (Api::GetRelation)0x5f8c79,
        (Api::GetBool)0x5f8cb0,
        (Api::GetAllianceTurn)0x5f8cf4,
        (Api::GetBool)0x5f8d2b,
        (Api::GetBool)0x5f8cd2,
        (Api::CheckRelationType)0x5f9101,
        (Api::CheckRelationType)0x5f912d,
        (Api::CheckRelationType)0x5f917d,
    },
    // Gog
    Api{
        (Api::GetRelation)0x5f788f,
        (Api::GetRelation)0x5f78c5,
        (Api::GetBool)0x5f78fc,
        (Api::GetAllianceTurn)0x5f7940,
        (Api::GetBool)0x5f7977,
        (Api::GetBool)0x5f791e,
        (Api::CheckRelationType)0x5f7d94,
        (Api::CheckRelationType)0x5f7dc0,
        (Api::CheckRelationType)0x5f7e10,
    },
    // Scenario Editor
    Api{
        (Api::GetRelation)0x4F7176,
        (Api::GetRelation)nullptr,
        (Api::GetBool)0x4F71F3,
        (Api::GetAllianceTurn)nullptr,
        (Api::GetBool)0x4F7237,
        (Api::GetBool)0x4F7215,
        (Api::CheckRelationType)nullptr,
        (Api::CheckRelationType)nullptr,
        (Api::CheckRelationType)nullptr,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

}