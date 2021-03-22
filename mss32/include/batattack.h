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

#ifndef BATATTACK_H
#define BATATTACK_H

#include "targetslist.h"

namespace game {

struct IMidgardObjectMap;
struct BattleMsgData;
struct CMidgardID;
struct LAttackClass;
struct IBatAttackVftable;
struct BattleAttackInfo;

/** Base class for attacks in battle. */
struct IBatAttack
{
    const IBatAttackVftable* vftable;
};

struct CBatAttackBase : public IBatAttack
{ };

struct IBatAttackVftable
{
    using Destructor = void(__thiscall*)(IBatAttack* thisptr, bool freeMemory);
    Destructor destructor;

    /** Returns true if unit with specified id can be attacked with this attack. */
    using CanPerform = bool(__thiscall*)(IBatAttack* thisptr,
                                         IMidgardObjectMap* objectMap,
                                         BattleMsgData* battleMsgData,
                                         CMidgardID* targetUnitId);
    CanPerform canPerform;

    /** Returns id of stack the attack can target: allies or enemies. */
    using GetTargetStackId = CMidgardID*(__thiscall*)(IBatAttack* thisptr,
                                                      CMidgardID* targetStackId,
                                                      BattleMsgData* battleMsgData);
    GetTargetStackId getTargetStackId;

    /** Fills attack targets list with positions of units in groups. */
    using FillTargetsList = void(__thiscall*)(IBatAttack* thisptr,
                                              IMidgardObjectMap* objectMap,
                                              BattleMsgData* battleMsgData,
                                              TargetsList* targetsList);
    FillTargetsList fillTargetsList;
    FillTargetsList fillTargetsList2;

    using Method5 = bool(__thiscall*)(IBatAttack* thisptr,
                                      IMidgardObjectMap* objectMap,
                                      BattleMsgData* battleMsgData,
                                      CMidgardID* unitId);
    Method5 method5;

    using CanMiss = bool(__thiscall*)(IBatAttack* thisptr,
                                      BattleMsgData* battleMsgData,
                                      CMidgardID* id);
    CanMiss canMiss;

    using Method7 = bool(__thiscall*)(IBatAttack* thisptr, int a2, int a3, int a4);
    Method7 method7;

    using IsImmune = bool(__thiscall*)(IBatAttack* thisptr,
                                       IMidgardObjectMap* objectMap,
                                       BattleMsgData* battleMsgData,
                                       CMidgardID* unitId);
    IsImmune isImmune;
    /** Attack action logic. */
    using OnAttack = void(__thiscall*)(IBatAttack* thisptr,
                                       IMidgardObjectMap* objectMap,
                                       BattleMsgData* battleMsgData,
                                       CMidgardID* targetUnitId,
                                       BattleAttackInfo** attackInfo);
    /** Called when attack misses the target. */
    OnAttack onMiss;

    using GetAttackClass = bool(__thiscall*)(IBatAttack* thisptr,
                                             int a2,
                                             int a3,
                                             LAttackClass* attackClass);
    GetAttackClass getAttackClass;

    using Method11 = bool(__thiscall*)(IBatAttack* thisptr,
                                       int a2,
                                       int a3,
                                       LAttackClass* attackClass);
    Method11 method11;

    /** Called when attack hits the target. */
    OnAttack onHit;

    /**
     * Called to determine if the attack can target dead units. Every known attack returns false,
     * except CBatAttackUsePotion that returns true if the potion is Revive potion.
     */
    using Method13 = bool(__thiscall*)(IBatAttack* thisptr,
                                       BattleMsgData* battleMsgData,
                                       IMidgardObjectMap* objectMap);
    Method13 method13;

    using UnknownMethod = bool(__thiscall*)(IBatAttack* thisptr, BattleMsgData* battleMsgData);

    /**
     * Called to determine if the attack can target dead units. Every known attack returns false,
     * except CBatAttackRevive.
     */
    UnknownMethod method14;

    /**
     * Called to determine if the attack can target dead units. Every known attack returns false,
     * except CBatAttackHeal that always returns true. It has additional checks for dead targets,
     * which succeed if the secondary attack is Revive.
     */
    UnknownMethod method15;

    /**
     * Called to determine if the attack can target dead units. Every known attack returns false,
     * except CBatAttackDoppelganger that returns true if the unit is not transformed yet. It has
     * additional check for dead targets (which seems to always fail though).
     */
    UnknownMethod method16;

    /**
     * Called to determine if the attack can target dead and retreated units. Every known attack
     * returns false, except CBatAttackSummon (because it can summon in slots with dead or
     * retreated units).
     */
    UnknownMethod method17;
};

static_assert(sizeof(IBatAttackVftable) == 18 * sizeof(void*),
              "IBatAttack vftable must have exactly 18 methods");

} // namespace game

#endif // BATATTACK_H
