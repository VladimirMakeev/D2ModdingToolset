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

#include "currencyview.h"
#include "unitimplview.h"
#include "unitview.h"
#include <lua.hpp>
#include <optional>
#include <sol/sol.hpp>

namespace hooks {

using Ids = std::vector<bindings::IdView>;
using GetId = std::function<bindings::IdView(const bindings::UnitView&, const bindings::IdView&)>;
using GetIds = std::function<sol::table(const bindings::UnitView&, const Ids&)>;
using GetInt = std::function<int(const bindings::UnitView&, int)>;
using GetIntParam = std::function<int(const bindings::UnitView&, int, int)>;
using GetUint8 = std::function<std::uint8_t(const bindings::UnitView&, std::uint8_t)>;
using GetBool = std::function<bool(const bindings::UnitView&, bool)>;
using GetBoolParam = std::function<bool(const bindings::UnitView&, int, bool)>;
using GetBank = std::function<bindings::CurrencyView(const bindings::UnitView&,
                                                     const bindings::CurrencyView&)>;
using CanApplyToUnit = std::function<bool(const bindings::UnitImplView&)>;
using CanApplyToUnitType = std::function<bool(int)>;
using GetDesc = std::function<bindings::IdView()>;
using IsLowerBoost = std::function<bool()>;

struct CustomModifierFunctions
{
    std::optional<sol::environment> env;
    std::optional<GetDesc> getModifierDescTxt;
    std::optional<CanApplyToUnit> canApplyToUnit;
    std::optional<CanApplyToUnitType> canApplyToUnitType;
    std::optional<IsLowerBoost> canApplyAsLowerSpell;
    std::optional<IsLowerBoost> canApplyAsBoostSpell;
    std::optional<GetId> getNameTxt;
    std::optional<GetId> getDescTxt;
    std::optional<GetInt> getHitPoint;
    std::optional<GetInt> getArmor;
    std::optional<GetInt> getDeathAnim;
    std::optional<GetInt> getRegen;
    std::optional<GetInt> getXpNext;
    std::optional<GetInt> getXpKilled;
    std::optional<GetIntParam> getImmuneToAttack;
    std::optional<GetIntParam> getImmuneToSource;
    std::optional<GetBool> getAtckTwice;
    std::optional<GetBank> getEnrollCost;
    std::optional<GetBank> getReviveCost;
    std::optional<GetBank> getHealCost;
    std::optional<GetBank> getTrainingCost;
    std::optional<GetInt> getMovement;
    std::optional<GetBoolParam> hasMovementBonus;
    std::optional<GetInt> getScout;
    std::optional<GetInt> getLeadership;
    std::optional<GetInt> getNegotiate;
    std::optional<GetBoolParam> hasAbility;
    std::optional<GetBool> getFastRetreat;
    std::optional<GetInt> getLowerCost;
    std::optional<GetId> getAttackId;
    std::optional<GetId> getAttack2Id;
    std::optional<GetInt> getAttackReach;
    std::optional<GetInt> getAttackInitiative;
    std::optional<GetId> getAttackNameTxt;
    std::optional<GetId> getAttack2NameTxt;
    std::optional<GetId> getAttackDescTxt;
    std::optional<GetId> getAttack2DescTxt;
    std::optional<GetUint8> getAttackDamRatio;
    std::optional<GetUint8> getAttack2DamRatio;
    std::optional<GetBool> getAttackDrRepeat;
    std::optional<GetBool> getAttack2DrRepeat;
    std::optional<GetBool> getAttackDrSplit;
    std::optional<GetBool> getAttack2DrSplit;
    std::optional<GetUint8> getAttackCritDamage;
    std::optional<GetUint8> getAttack2CritDamage;
    std::optional<GetUint8> getAttackCritPower;
    std::optional<GetUint8> getAttack2CritPower;
    std::optional<GetInt> getAttackClass;
    std::optional<GetInt> getAttack2Class;
    std::optional<GetInt> getAttackSource;
    std::optional<GetInt> getAttack2Source;
    std::optional<GetInt> getAttackPower;
    std::optional<GetInt> getAttack2Power;
    std::optional<GetInt> getAttackDamage;
    std::optional<GetInt> getAttack2Damage;
    std::optional<GetInt> getAttackHeal;
    std::optional<GetInt> getAttack2Heal;
    std::optional<GetIntParam> getAttackDrain;
    std::optional<GetIntParam> getAttack2Drain;
    std::optional<GetInt> getAttackLevel;
    std::optional<GetInt> getAttack2Level;
    std::optional<GetBool> getAttackInfinite;
    std::optional<GetBool> getAttack2Infinite;
    std::optional<GetBool> getAttackCritHit;
    std::optional<GetBool> getAttack2CritHit;
    std::optional<GetIds> getAttackWards;
    std::optional<GetIds> getAttack2Wards;
};

CustomModifierFunctions* createCustomModifierFunctions(const char* scriptFileName);
void deleteCustomModifierFunctions(CustomModifierFunctions* value);

} // namespace hooks

#endif // CUSTOMMODIFIERFUNCTIONS_H
