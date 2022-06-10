/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef ENCUNITDESCRIPTOR_H
#define ENCUNITDESCRIPTOR_H

#include "attackclasscat.h"
#include "attacksourcecat.h"
#include "d2list.h"
#include "idvector.h"
#include "leaderabilitycat.h"

namespace game {

struct IEncUnitDescriptorVftable;
struct LAttackReach;
struct LLeaderAbility;

struct IEncUnitDescriptor
{
    const IEncUnitDescriptorVftable* vftable;
};

struct IEncUnitDescriptorVftable
{
    using Destructor = void(__thiscall*)(IEncUnitDescriptor* thisptr, bool freeMemory);
    Destructor destructor;

    using GetBool = bool(__thiscall*)(IEncUnitDescriptor* thisptr);
    GetBool method1;

    using GetInt = int(__thiscall*)(IEncUnitDescriptor* thisptr);
    GetInt getHp;

    GetInt getHpMax;

    using GetId = CMidgardID*(__thiscall*)(IEncUnitDescriptor* thisptr, CMidgardID* value);
    GetId getAttackId;

    using GetAttackClass = const LAttackClass*(__thiscall*)(IEncUnitDescriptor* thisptr);
    GetAttackClass getAttackClass;

    using GetStr = const char*(__thiscall*)(IEncUnitDescriptor* thisptr);
    GetStr getAttackName;

    GetInt getAttackPower;

    GetInt getAttackLevel;

    using GetAttackSource = const LAttackSource*(__thiscall*)(IEncUnitDescriptor* thisptr);
    GetAttackSource getAttackSource;

    using GetAttackReach = const LAttackReach*(__thiscall*)(IEncUnitDescriptor* thisptr);
    GetAttackReach getAttackReach;

    GetInt getAttackDamageOrHeal;

    GetInt getAttackInitiative;

    GetId getAltAttackId;

    GetStr getAltAttackName;

    GetInt getAltAttackPower;

    GetId getAttack2Id;

    GetStr getAttack2Name;

    GetInt getAttack2Power;

    GetBool attacksTwice;

    GetInt getLeadership;

    GetInt getNbBattle;

    GetBool isUnitInStack;

    GetInt getMovement;

    GetBool isUnitLeader;

    GetId getGlobalUnitImplId;

    GetStr getUnitName;

    GetStr getUnitDescription;

    GetInt getUnitLevel;

    GetInt getUnitArmor;

    GetBool isUnitAtMaxLevel;

    GetInt getUnitCurrentXp;

    GetInt getUnitXpNext;

    GetBool canUnitLevelUp;

    using GetUnitRequiredBuildings = void(__thiscall*)(IEncUnitDescriptor* thisptr,
                                                       IdVector* buildings);
    GetUnitRequiredBuildings getUnitRequiredBuildings;

    GetBool getUnitDynLevel;

    GetBool unknownMethod;

    GetBool getLeaderFlyAbility;

    using GetLeaderAbilities = void(__thiscall*)(IEncUnitDescriptor* thisptr,
                                                 List<LLeaderAbility>* abilities);
    GetLeaderAbilities getLeaderAbilities;

    using GetAttackClasses = void(__thiscall*)(IEncUnitDescriptor* thisptr,
                                               List<LAttackClass>* attackClasses);
    GetAttackClasses getAttackClassesUnitIsImmuneTo;

    using GetAttackSources = void(__thiscall*)(IEncUnitDescriptor* thisptr,
                                               List<LAttackSource>* attackSources);
    GetAttackSources getAttackSourcesUnitIsImmuneTo;

    GetAttackClasses getAttackClassesUnitIsResistantTo;

    GetAttackSources getAttackSourcesUnitIsResistantTo;
};

assert_vftable_size(IEncUnitDescriptorVftable, 43);

} // namespace game

#endif // ENCUNITDESCRIPTOR_H
