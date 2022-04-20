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
        (Api::MissingValueException)0x5965ac,
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
        (Api::ReadBoolValue)0x596911,
        (Api::GetName)0x515760,
        (Api::Eof)0x5157ec,
        (Api::Next)0x51580e,
    },
    // Russobit
    Api{
        (Api::FindBuildingCategory)0x58c041,
        (Api::FindUnitBranchCategory)0x58c0bf,
        (Api::ReadUnitLevel)0x58be7c,
        (Api::MissingValueException)0x5965ac,
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
        (Api::ReadBoolValue)0x596911,
        (Api::GetName)0x515760,
        (Api::Eof)0x5157ec,
        (Api::Next)0x51580e,
    },
    // Gog
    Api{
        (Api::FindBuildingCategory)0x58b1cf,
        (Api::FindUnitBranchCategory)0x58b24d,
        (Api::ReadUnitLevel)0x58afd7,
        (Api::MissingValueException)0x5956d1,
        (Api::DuplicateRecordException)0x59c710,
        (Api::ReadId)0x595a5a,
        (Api::ReadText)0x595c8c,
        (Api::FindAttackClass)0x59ed57,
        (Api::FindAttackSource)0x583144,
        (Api::FindAttackReach)0x5a5c92,
        (Api::ReadAttackIntValue)0x5a5655,
        (Api::ReadPower)0x5a5708,
        (Api::ReadAttackIntValue)0x5a5751,
        (Api::ReadAttackIntValue)0x5a5794,
        (Api::ReadAttackLevel)0x5a57cb,
        (Api::ReadBoolValue)0x5a5870,
        (Api::ReadIntWithBoundsCheck)0x5959f1,
        (Api::ReadBoolValue)0x595a36,
        (Api::GetName)0x514c59,
        (Api::Eof)0x514ce5,
        (Api::Next)0x514d07,
    },
    // Scenario Editor
    Api{
        (Api::FindBuildingCategory)0x538bf7,
        (Api::FindUnitBranchCategory)0x538c75,
        (Api::ReadUnitLevel)0x538a1b,
        (Api::MissingValueException)0x536b7e,
        (Api::DuplicateRecordException)0x53f9b2,
        (Api::ReadId)0x536f07,
        (Api::ReadText)0x537139,
        (Api::FindAttackClass)0x547d4a,
        (Api::FindAttackSource)0x5325c0,
        (Api::FindAttackReach)0x54ec66,
        (Api::ReadAttackIntValue)0x54e637,
        (Api::ReadPower)0x54e6ea,
        (Api::ReadAttackIntValue)0x54e733,
        (Api::ReadAttackIntValue)0x54e776,
        (Api::ReadAttackLevel)0x54e7ad,
        (Api::ReadBoolValue)0x54e852,
        (Api::ReadIntWithBoundsCheck)0x536e9e,
        (Api::ReadBoolValue)0x536ee3,
        (Api::GetName)0x4ac3a3,
        (Api::Eof)0x4ac42f,
        (Api::Next)0x4ac451,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CDBTableApi
