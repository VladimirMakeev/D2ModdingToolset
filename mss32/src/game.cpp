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
        (GetAttackById)0x645df6,
        (IsUnitImmuneToAttack)0x65baa8,
        (AttackClassToNumber)0x623e12,
        (AttackClassToString)0x5c7af0,
        (AttackSourceToNumber)0x622dfe,
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
        (GetUnitPositionInGroup)0x65fb1e,
        (GetSummonUnitImplIdByAttack)0x661fe0,
        (GetSummonUnitImplId)0x664b5c,
        (GetAttackImplMagic)0x647561,
        (GetUnitHealAttackNumber)0x6283fb,
        (GetAttackQtyDamageOrHeal)0x61efc4,
        (ComputeUnitEffectiveHp)0x5d0053,
        (ApplyDynUpgradeToAttackData)0x59a76b,
        (ComputeUnitDynUpgrade)0x5998fc,
        (ShowMovementPath)0x4cd533,
        (GetMidgardPlan)0x5f756f,
        (CastUnitImplToNoble)0x41b8cc,
        (GetBlockingPathNearbyStackId)0x5ce204,
        (GetFortOrRuinEntrance)0x4cdb5a,
        (ShowImageOnGround)0x5adf66,
        (StackCanMoveToPosition)0x4cdae4,
        (IsWaterTileSurroundedByWater)0x5e773e,
        (GetStackPositionById)0x5eecf2,
    },
    // Russobit
    Functions{
        (RespopupInitFunc)0x4893ad,
        (ToggleShowBannersInitFunc)0x5b4015,
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
        (GetAttackById)0x645df6,
        (IsUnitImmuneToAttack)0x65baa8,
        (AttackClassToNumber)0x623e12,
        (AttackClassToString)0x5c7af0,
        (AttackSourceToNumber)0x622dfe,
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
        (GetUnitPositionInGroup)0x65fb1e,
        (GetSummonUnitImplIdByAttack)0x661fe0,
        (GetSummonUnitImplId)0x664b5c,
        (GetAttackImplMagic)0x647561,
        (GetUnitHealAttackNumber)0x6283fb,
        (GetAttackQtyDamageOrHeal)0x61efc4,
        (ComputeUnitEffectiveHp)0x5d0053,
        (ApplyDynUpgradeToAttackData)0x59a76b,
        (ComputeUnitDynUpgrade)0x5998fc,
        (ShowMovementPath)0x4cd533,
        (GetMidgardPlan)0x5f756f,
        (CastUnitImplToNoble)0x41b8cc,
        (GetBlockingPathNearbyStackId)0x5ce204,
        (GetFortOrRuinEntrance)0x4cdb5a,
        (ShowImageOnGround)0x5adf66,
        (StackCanMoveToPosition)0x4cdae4,
        (IsWaterTileSurroundedByWater)0x5e773e,
        (GetStackPositionById)0x5eecf2,
    },
    // Gog
    Functions{
        (RespopupInitFunc)0x488f96,
        (ToggleShowBannersInitFunc)0x5b32db,
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
        (GetAttackById)0x644626,
        (IsUnitImmuneToAttack)0x65a528,
        (AttackClassToNumber)0x6229a2,
        (AttackClassToString)0x5c6ad9,
        (AttackSourceToNumber)0x62198e,
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
        (GetUnitPositionInGroup)0x65e59e,
        (GetSummonUnitImplIdByAttack)0x660a60,
        (GetSummonUnitImplId)0x6635dc,
        (GetAttackImplMagic)0x645d91,
        (GetUnitHealAttackNumber)0x626f3b,
        (GetAttackQtyDamageOrHeal)0x61db00,
        (ComputeUnitEffectiveHp)0x5cef84,
        (ApplyDynUpgradeToAttackData)0x5998f5,
        (ComputeUnitDynUpgrade)0x598a86,
        (ShowMovementPath)0x4ccc2c,
        (GetMidgardPlan)0x5f61f2,
        (CastUnitImplToNoble)0x41b399,
        (GetBlockingPathNearbyStackId)0x5cd11f,
        (GetFortOrRuinEntrance)0x4cd253,
        (ShowImageOnGround)0x5ad1ee,
        (StackCanMoveToPosition)0x4cd1dd,
        (IsWaterTileSurroundedByWater)0x5e6453,
        (GetStackPositionById)0x5ed9b2,
    },
    // Scenario Editor
    Functions{
        (RespopupInitFunc)nullptr,
        (ToggleShowBannersInitFunc)nullptr,
        (AddUnitToHireList)nullptr,
        (AddSideshowUnitToHireList)nullptr,
        (AddPlayerUnitsToHireList)nullptr,
        (CreateBuildingType)0x538b2d,
        (AddObjectAndCheckDuplicates)0x53cd9c,
        (ChooseUnitLane)nullptr,
        (GetPlayerIdFromPhase)nullptr,
        (GetObjectMapFromPhase)nullptr,
        (GetLordByPlayer)nullptr,
        (IsTurnValid)0x52d22e,
        (GetAllyOrEnemyStackId)nullptr,
        (FindUnitById)nullptr,
        (CastUnitImplToSoldier)nullptr,
        (CreateBatAttack)nullptr,
        (GetAttackById)nullptr,
        (IsUnitImmuneToAttack)nullptr,
        (AttackClassToNumber)0x525282,
        (AttackClassToString)nullptr,
        (AttackSourceToNumber)0x52426e,
        (GetStackFortRuinGroup)nullptr,
        (DeletePlayerBuildings)nullptr,
        (GetInterfaceText)nullptr,
        (ComputePlayerDailyIncome)nullptr,
        (ComputeDamage)nullptr,
        (MarkMapPosition)nullptr,
        (GetUnitLevelByImplId)nullptr,
        (GetAttackAccuracy)nullptr,
        (IsGroupOwnerPlayerHuman)nullptr,
        (AttackShouldMiss)nullptr,
        (GenerateRandomNumber)nullptr,
        (GetUnitPositionInGroup)nullptr,
        (GetSummonUnitImplIdByAttack)nullptr,
        (GetSummonUnitImplId)nullptr,
        (GetAttackImplMagic)nullptr,
        (GetUnitHealAttackNumber)nullptr,
        (GetAttackQtyDamageOrHeal)0x5229dc,
        (ComputeUnitEffectiveHp)nullptr,
        (ApplyDynUpgradeToAttackData)0x545355,
        (ComputeUnitDynUpgrade)0x5444e6,
        (ShowMovementPath)nullptr,
        (GetMidgardPlan)nullptr,
        (CastUnitImplToNoble)nullptr,
        (GetBlockingPathNearbyStackId)nullptr,
        (GetFortOrRuinEntrance)nullptr,
        (ShowImageOnGround)nullptr,
        (StackCanMoveToPosition)nullptr,
        (IsWaterTileSurroundedByWater)nullptr,
        (GetStackPositionById)nullptr,
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
