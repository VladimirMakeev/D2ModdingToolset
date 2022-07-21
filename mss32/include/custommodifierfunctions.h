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

#ifndef CUSTOMMODIFIERFUNCTIONS_H
#define CUSTOMMODIFIERFUNCTIONS_H

#include <lua.hpp>
#include <optional>
#include <sol/sol.hpp>
#include <string>

namespace hooks {

struct CustomModifierFunctions
{
    void initialize(const std::string& scriptFileName);

    std::optional<sol::environment> environment;
    std::optional<sol::function> canApplyToUnit;
    std::optional<sol::function> canApplyToUnitType;
    std::optional<sol::function> canApplyAsLowerSpell;
    std::optional<sol::function> canApplyAsBoostSpell;
    std::optional<sol::function> getModifierDisplay;
    std::optional<sol::function> getModifierDescTxt;
    std::optional<sol::function> getModifierIconName;
    std::optional<sol::function> getNameTxt;
    std::optional<sol::function> getDescTxt;
    std::optional<sol::function> getHitPoint;
    std::optional<sol::function> getArmor;
    std::optional<sol::function> getDeathAnim;
    std::optional<sol::function> getRegen;
    std::optional<sol::function> getXpNext;
    std::optional<sol::function> getXpKilled;
    std::optional<sol::function> getImmuneToAttack;
    std::optional<sol::function> getImmuneToSource;
    std::optional<sol::function> getAtckTwice;
    std::optional<sol::function> getEnrollCost;
    std::optional<sol::function> getReviveCost;
    std::optional<sol::function> getHealCost;
    std::optional<sol::function> getTrainingCost;
    std::optional<sol::function> getMovement;
    std::optional<sol::function> hasMovementBonus;
    std::optional<sol::function> getScout;
    std::optional<sol::function> getLeadership;
    std::optional<sol::function> getNegotiate;
    std::optional<sol::function> hasAbility;
    std::optional<sol::function> getFastRetreat;
    std::optional<sol::function> getLowerCost;
    std::optional<sol::function> getAttackId;
    std::optional<sol::function> getAttack2Id;
    std::optional<sol::function> getAttackReach;
    std::optional<sol::function> getAttackInitiative;
    std::optional<sol::function> getAttackNameTxt;
    std::optional<sol::function> getAttack2NameTxt;
    std::optional<sol::function> getAttackDescTxt;
    std::optional<sol::function> getAttack2DescTxt;
    std::optional<sol::function> getAttackDamRatio;
    std::optional<sol::function> getAttack2DamRatio;
    std::optional<sol::function> getAttackDrRepeat;
    std::optional<sol::function> getAttack2DrRepeat;
    std::optional<sol::function> getAttackDrSplit;
    std::optional<sol::function> getAttack2DrSplit;
    std::optional<sol::function> getAttackCritDamage;
    std::optional<sol::function> getAttack2CritDamage;
    std::optional<sol::function> getAttackCritPower;
    std::optional<sol::function> getAttack2CritPower;
    std::optional<sol::function> getAttackClass;
    std::optional<sol::function> getAttack2Class;
    std::optional<sol::function> getAttackSource;
    std::optional<sol::function> getAttack2Source;
    std::optional<sol::function> getAttackPower;
    std::optional<sol::function> getAttack2Power;
    std::optional<sol::function> getAttackDamage;
    std::optional<sol::function> getAttack2Damage;
    std::optional<sol::function> getAttackHeal;
    std::optional<sol::function> getAttack2Heal;
    std::optional<sol::function> getAttackDrain;
    std::optional<sol::function> getAttack2Drain;
    std::optional<sol::function> getAttackLevel;
    std::optional<sol::function> getAttack2Level;
    std::optional<sol::function> getAttackInfinite;
    std::optional<sol::function> getAttack2Infinite;
    std::optional<sol::function> getAttackCritHit;
    std::optional<sol::function> getAttack2CritHit;
    std::optional<sol::function> getAttackWards;
    std::optional<sol::function> getAttack2Wards;
};

} // namespace hooks

#endif // CUSTOMMODIFIERFUNCTIONS_H
