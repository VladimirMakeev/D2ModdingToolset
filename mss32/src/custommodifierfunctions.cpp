/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "custommodifierfunctions.h"
#include "currencyview.h"
#include "idview.h"
#include "scripts.h"
#include "unitimplview.h"
#include "unitview.h"
#include "utils.h"

namespace hooks {

CustomModifierFunctions::CustomModifierFunctions(const std::string& scriptFileName)
{
    environment = executeScriptFile(modifiersFolder() / scriptFileName, false, true);
    if (!environment)
        return;

#define FUNCTION(_NAME_) this->##_NAME_ = getScriptFunction(env, #_NAME_);

    const auto& env = *environment;
    FUNCTION(canApplyToUnit)
    FUNCTION(canApplyToUnitType)
    FUNCTION(canApplyAsLowerSpell)
    FUNCTION(canApplyAsBoostSpell)
    FUNCTION(getModifierDisplay)
    FUNCTION(getModifierDescTxt)
    FUNCTION(getModifierIconName)
    FUNCTION(getNameTxt)
    FUNCTION(getDescTxt)
    FUNCTION(getHitPoint)
    FUNCTION(getArmor)
    FUNCTION(getDeathAnim)
    FUNCTION(getRegen)
    FUNCTION(getXpNext)
    FUNCTION(getXpKilled)
    FUNCTION(getImmuneToAttack)
    FUNCTION(getImmuneToSource)
    FUNCTION(getAtckTwice)
    FUNCTION(getEnrollCost)
    FUNCTION(getReviveCost)
    FUNCTION(getHealCost)
    FUNCTION(getTrainingCost)
    FUNCTION(getMovement)
    FUNCTION(hasMovementBonus)
    FUNCTION(getScout)
    FUNCTION(getLeadership)
    FUNCTION(getNegotiate)
    FUNCTION(hasAbility)
    FUNCTION(getFastRetreat)
    FUNCTION(getLowerCost)
    FUNCTION(getAttackId)
    FUNCTION(getAttack2Id)
    FUNCTION(getAltAttackId)
    FUNCTION(getAttackReach)
    FUNCTION(getAttackInitiative)
    FUNCTION(getAttackNameTxt)
    FUNCTION(getAttack2NameTxt)
    FUNCTION(getAttackDescTxt)
    FUNCTION(getAttack2DescTxt)
    FUNCTION(getAttackDamRatio)
    FUNCTION(getAttack2DamRatio)
    FUNCTION(getAttackDrRepeat)
    FUNCTION(getAttack2DrRepeat)
    FUNCTION(getAttackDrSplit)
    FUNCTION(getAttack2DrSplit)
    FUNCTION(getAttackCritDamage)
    FUNCTION(getAttack2CritDamage)
    FUNCTION(getAttackCritPower)
    FUNCTION(getAttack2CritPower)
    FUNCTION(getAttackClass)
    FUNCTION(getAttack2Class)
    FUNCTION(getAttackSource)
    FUNCTION(getAttack2Source)
    FUNCTION(getAttackPower)
    FUNCTION(getAttack2Power)
    FUNCTION(getAttackDamage)
    FUNCTION(getAttack2Damage)
    FUNCTION(getAttackHeal)
    FUNCTION(getAttack2Heal)
    FUNCTION(getAttackDrain)
    FUNCTION(getAttack2Drain)
    FUNCTION(getAttackLevel)
    FUNCTION(getAttack2Level)
    FUNCTION(getAttackInfinite)
    FUNCTION(getAttack2Infinite)
    FUNCTION(getAttackCritHit)
    FUNCTION(getAttack2CritHit)
    FUNCTION(getAttackWards)
    FUNCTION(getAttack2Wards)
}

} // namespace hooks
