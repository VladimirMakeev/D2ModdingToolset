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

#include "game.h"
#include "version.h"
#include <array>

namespace game {

// clang-format off
static std::array<Functions, 4> functions = {{
    // Akella
    Functions{
        (RespopupInitFunc)0x4893ad,
        (ToggleShowBannersInitFunc)0x5b4015,
        (ProcessUnitModifiersFunc)0x5eac81,
        (AddUnitToHireList)0x5d5bb4,
        (AddSideshowUnitToHireList)0x5d5ab6,
        (AddPlayerUnitsToHireList)0x5d59e8,
        (CreateBuildingType)0x58bf67,
        (AddObjectAndCheckDuplicates)0x59c0a8,
        (ChooseUnitLane)0x5d6abb,
        (GetPlayerIdFromPhase)0x404e71,
        (GetObjectMapFromPhase)0x404f06,
        (GetLordByPlayer)0x5e6323,
        (IsTurnValid)0x5879f3,
        (GetAllyOrEnemyStackId)0x65afb2,
        (FindUnitById)0x625180,
        (CastUnitImplToSoldier)0x40a79d,
        (CreateBatAttack)0x64630e,
        (GetAttackByIdAndCheckTransfomed)0x64752d,
        (IsUnitImmuneToAttack)0x65baa8,
        (AttackClassToNumber)0x623e12,
        (AttackClassToString)0x5c7af0,
        (GetStackFortRuinGroup)0x5f6304,
        (DeletePlayerBuildings)0x422498,
        (GetInterfaceText)0x5c8f38,
        (ComputePlayerDailyIncome)0x5db82a,
        (ComputeDamage)0x65ccc7,
        (MarkMapPosition)0x5cdf6f,
        (GetUnitLevelByImplId)0x599581,
        (GetAttackAccuracy)0x628b0a,
        (IsGroupOwnerPlayerHuman)0x628165,
        (AttackShouldMiss)0x626b5f,
        (GenerateRandomNumber)0x548e10,
    },
    // Russobit
    Functions{
        (RespopupInitFunc)0x4893ad,
        (ToggleShowBannersInitFunc)0x5b4015,
        (ProcessUnitModifiersFunc)0x5eac81,
        (AddUnitToHireList)0x5d5bb4,
        (AddSideshowUnitToHireList)0x5d5ab6,
        (AddPlayerUnitsToHireList)0x5d59e8,
        (CreateBuildingType)0x58bf67,
        (AddObjectAndCheckDuplicates)0x59c0a8,
        (ChooseUnitLane)0x5d6abb,
        (GetPlayerIdFromPhase)0x404e71,
        (GetObjectMapFromPhase)0x404f06,
        (GetLordByPlayer)0x5e6323,
        (IsTurnValid)0x5879f3,
        (GetAllyOrEnemyStackId)0x65afb2,
        (FindUnitById)0x625180,
        (CastUnitImplToSoldier)0x40a79d,
        (CreateBatAttack)0x64630e,
        (GetAttackByIdAndCheckTransfomed)0x64752d,
        (IsUnitImmuneToAttack)0x65baa8,
        (AttackClassToNumber)0x623e12,
        (AttackClassToString)0x5c7af0,
        (GetStackFortRuinGroup)0x5f6304,
        (DeletePlayerBuildings)0x422498,
        (GetInterfaceText)0x5c8f38,
        (ComputePlayerDailyIncome)0x5db82a,
        (ComputeDamage)0x65ccc7,
        (MarkMapPosition)0x5cdf6f,
        (GetUnitLevelByImplId)0x599581,
        (GetAttackAccuracy)0x628b0a,
        (IsGroupOwnerPlayerHuman)0x628165,
        (AttackShouldMiss)0x626b5f,
        (GenerateRandomNumber)0x548e10,
    },
    // Gog
    Functions{
        (RespopupInitFunc)0x488f96,
        (ToggleShowBannersInitFunc)0x5b32db,
        (ProcessUnitModifiersFunc)0x5e9984,
        (AddUnitToHireList)0x5d4add,
        (AddSideshowUnitToHireList)0x5d49df,
        (AddPlayerUnitsToHireList)0x5d4911,
        (CreateBuildingType)0x58b0c2,
        (AddObjectAndCheckDuplicates)0x59b275,
        (ChooseUnitLane)0x5d59e4,
        (GetPlayerIdFromPhase)0x404af9,
        (GetObjectMapFromPhase)0x404b8e,
        (GetLordByPlayer)0x5e5038,
        (IsTurnValid)0x586ba8,
        (GetAllyOrEnemyStackId)0x659a32,
        (FindUnitById)0x623cc0,
        (CastUnitImplToSoldier)0x40a3f9,
        (CreateBatAttack)0x644b3e,
        (GetAttackByIdAndCheckTransfomed)0x645d5d,
        (IsUnitImmuneToAttack)0x65a528,
        (AttackClassToNumber)0x6229a2,
        (AttackClassToString)0x5c6ad9,
        (GetStackFortRuinGroup)0x5f4f87,
        (DeletePlayerBuildings)0x421fb6,
        (GetInterfaceText)0x5c7f06,
        (ComputePlayerDailyIncome)0x5da55f,
        (ComputeDamage)0x65b747,
        (MarkMapPosition)0x5cce8b,
        (GetUnitLevelByImplId)0x59870b,
        (GetAttackAccuracy)0x62764a,
        (IsGroupOwnerPlayerHuman)0x626ca5,
        (AttackShouldMiss)0x62569f,
        (GenerateRandomNumber)0x54851f,
    },
    // Scenario Editor
    Functions{
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        (CreateBuildingType)0x538b2d,
        (AddObjectAndCheckDuplicates)0x53cd9c,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        (IsTurnValid)0x52d22e,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    }
}};

static std::array<Variables, 4> variables = {{
    // Akella
    Variables{
        (int*)0x837ac8,
        (unsigned char*)0x837acc
    },
    // Russobit
    Variables{
        (int*)0x837ac8,
        (unsigned char*)0x837acc
    },
    // Gog
    Variables{
        (int*)0x835a78,
        (unsigned char*)0x835a7c
    },
    // Scenario Editor
    Variables{
        nullptr,
        nullptr
    },
}};
// clang-format on

Functions& gameFunctions()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

Variables& gameVariables()
{
    return variables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game
