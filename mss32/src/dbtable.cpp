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

#include "dbtable.h"
#include "version.h"
#include <array>

namespace game::CDBTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::FindBuildingCategory)0x58c041,
        (Api::FindUnitBranchCategory)0x58c0bf,
        (Api::ReadUnitLevel)0x58be7c,
        (Api::DuplicateRecordException)0x59d474,
        (Api::ReadId)0x596935,
        (Api::ReadText)0x596b67,
        (Api::FindAttackClass)0x59fad5,
        (Api::FindAttackSource)0x583f7d,
        (Api::FindAttackReach)0x5a6a31,
        (Api::ReadAttackIntValue)0x5a6409,
        (Api::ReadPower)0x5a64bc,
        (Api::ReadAttackIntValue)0x5a6505,
        (Api::ReadAttackIntValue)0x5a6548,
        (Api::ReadAttackLevel)0x5a657f,
        (Api::ReadBoolValue)0x5a6624,
        (Api::ReadIntWithBoundsCheck)0x5968cc,
        (Api::ReadBoolValue)0x596911
    },
    // Russobit
    Api{
        (Api::FindBuildingCategory)0x58c041,
        (Api::FindUnitBranchCategory)0x58c0bf,
        (Api::ReadUnitLevel)0x58be7c,
        (Api::DuplicateRecordException)0x59d474
    },
    // Gog
    Api{
        (Api::FindBuildingCategory)0x58b1cf,
        (Api::FindUnitBranchCategory)0x58b24d,
        (Api::ReadUnitLevel)0x58afd7,
        (Api::DuplicateRecordException)0x59c710
    },
    // Scenario Editor
    Api{
        (Api::FindBuildingCategory)0x538bf7,
        (Api::FindUnitBranchCategory)0x538c75,
        (Api::ReadUnitLevel)0x538a1b,
        (Api::DuplicateRecordException)0x53f9b2
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CDBTableApi
