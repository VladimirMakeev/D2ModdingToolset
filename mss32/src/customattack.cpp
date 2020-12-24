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

#include "customattack.h"
#include "attackclasscat.h"
#include "customattacks.h"
#include "game.h"
#include "log.h"
#include "mempool.h"

namespace hooks {

void __fastcall customAttackDtor(CustomAttack* attack, int /*%edx*/, bool freeMemory)
{
    logDebug("newAttackType.log", "CustomAttack d-tor");
    if (freeMemory) {
        game::Memory::get().freeNonZero(attack);
    }
}

bool __fastcall customAttackCanPerform(CustomAttack* thisptr,
                                       int /*%edx*/,
                                       game::IMidgardObjectMap* objectMap,
                                       game::BattleMsgData* battleMsgData,
                                       game::CMidgardID* unitId)
{
    logDebug("newAttackType.log", "CustomAttack canPerform");
    return true;
}

game::CMidgardID* __fastcall customAttackGetTargetStackId(CustomAttack* thisptr,
                                                          int /*%edx*/,
                                                          game::CMidgardID* targetStackId,
                                                          game::BattleMsgData* battleMsgData)
{
    logDebug("newAttackType.log", "CustomAttack getTargetStackId");
    game::gameFunctions().getAllyOrEnemyStackId(targetStackId, battleMsgData, &thisptr->id1, false);
    return targetStackId;
}

void __fastcall customAttackFillTargetsList(CustomAttack* thisptr,
                                            int /*%edx*/,
                                            game::IMidgardObjectMap* objectMap,
                                            game::BattleMsgData* battleMsgData,
                                            game::TargetsList* targetsList)
{
    logDebug("newAttackType.log", "CustomAttack fillTargetsList");
    game::TargetsListApi::get().fill(objectMap, battleMsgData, thisptr, &thisptr->id1,
                                     &thisptr->id2, false, targetsList, true);
}

void __fastcall customAttackFillTargetsList2(CustomAttack* thisptr,
                                             int /*%edx*/,
                                             game::IMidgardObjectMap* objectMap,
                                             game::BattleMsgData* battleMsgData,
                                             game::TargetsList* targetsList)
{
    logDebug("newAttackType.log", "CustomAttack fillTargetsList2");
    // do nothing
}

bool __fastcall customAttackMethod5(CustomAttack* thisptr,
                                    int /*%edx*/,
                                    game::IMidgardObjectMap* objectMap,
                                    game::BattleMsgData* battleMsgData,
                                    game::CMidgardID* unitId)
{
    logDebug("newAttackType.log", "CustomAttack method 5");
    return thisptr->vftable->canPerform(thisptr, objectMap, battleMsgData, unitId);
}

bool __fastcall customAttackMethod6(CustomAttack* thisptr,
                                    int /*%edx*/,
                                    game::BattleMsgData* battleMsgData,
                                    game::CMidgardID* id)
{
    logDebug("newAttackType.log", "CustomAttack method 6");
    return true;
}

bool __fastcall customAttackMethod7(CustomAttack* thisptr, int /*%edx*/, int a2, int a3, int a4)
{
    logDebug("newAttackType.log", "CustomAttack method 7");
    return false;
}

bool __fastcall customAttackIsImmune(CustomAttack* thisptr,
                                     int /*%edx*/,
                                     game::IMidgardObjectMap* objectMap,
                                     game::BattleMsgData* battleMsgData,
                                     game::CMidgardID* unitId)
{
    logDebug("newAttackType.log", "CustomAttack isImmune");
    return game::gameFunctions().isUnitImmuneToAttack(objectMap, battleMsgData, unitId,
                                                      thisptr->attack, false);
}

void __fastcall customAttackMethod8(CustomAttack* thisptr,
                                    int /*%edx*/,
                                    int a2,
                                    int a3,
                                    int a4,
                                    int a5)
{
    logDebug("newAttackType.log", "CustomAttack method 8");
    // do nothing
}

bool __fastcall customAttackGetAttackClass(CustomAttack* thisptr,
                                           int /*%edx*/,
                                           int a2,
                                           int a3,
                                           game::LAttackClass* attackClass)
{
    logDebug("newAttackType.log", "CustomAttack getAttackClass");
    attackClass->id = customAttackClass.id;
    attackClass->table = customAttackClass.table;
    return true;
}

bool __fastcall customAttackMethod9(CustomAttack* thisptr,
                                    int /*%edx*/,
                                    int a2,
                                    int a3,
                                    game::LAttackClass* attackClass)
{
    logDebug("newAttackType.log", "CustomAttack method 9");
    return thisptr->vftable->method9(thisptr, a2, a3, attackClass);
}

void __fastcall customAttackDoAttack(CustomAttack* thisptr,
                                     int /*%edx*/,
                                     game::IMidgardObjectMap* objectMap,
                                     game::BattleMsgData* battleMsgData,
                                     game::CMidgardID* unitId,
                                     void* a5)
{
    logDebug("newAttackType.log", "CustomAttack doAttack");
}

bool __fastcall customAttackMethod10(CustomAttack* thisptr,
                                     int /*%edx*/,
                                     game::BattleMsgData* battleMsgData,
                                     game::IMidgardObjectMap* objectMap)
{
    logDebug("newAttackType.log", "CustomAttack method 10");
    return false;
}

bool __fastcall customAttackMethod11(CustomAttack* thisptr,
                                     int /*%edx*/,
                                     game::BattleMsgData* battleMsgData)
{
    logDebug("newAttackType.log", "CustomAttack method 11");
    return false;
}

bool __fastcall customAttackMethod12(CustomAttack* thisptr,
                                     int /*%edx*/,
                                     game::BattleMsgData* battleMsgData)
{
    logDebug("newAttackType.log", "CustomAttack method 12");
    return false;
}

bool __fastcall customAttackMethod13(CustomAttack* thisptr,
                                     int /*%edx*/,
                                     game::BattleMsgData* battleMsgData)
{
    logDebug("newAttackType.log", "CustomAttack method 13");
    return false;
}

bool __fastcall customAttackMethod14(CustomAttack* thisptr,
                                     int /*%edx*/,
                                     game::BattleMsgData* battleMsgData)
{
    logDebug("newAttackType.log", "CustomAttack method 14");
    return false;
}

// clang-format off
static const game::IBatAttackVftable customAttackVftable{
    (game::IBatAttackVftable::Destructor)customAttackDtor,
    (game::IBatAttackVftable::CanPerform)customAttackCanPerform,
    (game::IBatAttackVftable::GetTargetStackId)customAttackGetTargetStackId,
    (game::IBatAttackVftable::FillTargetsList)customAttackFillTargetsList,
    (game::IBatAttackVftable::FillTargetsList)customAttackFillTargetsList2,
    (game::IBatAttackVftable::Method5)customAttackMethod5,
    (game::IBatAttackVftable::Method6)customAttackMethod6,
    (game::IBatAttackVftable::Method7)customAttackMethod7,
    (game::IBatAttackVftable::IsImmune)customAttackIsImmune,
    (game::IBatAttackVftable::OnAttack)customAttackMethod8,
    (game::IBatAttackVftable::GetAttackClass)customAttackGetAttackClass,
    (game::IBatAttackVftable::Method9)customAttackMethod9,
    (game::IBatAttackVftable::OnAttack)customAttackDoAttack,
    (game::IBatAttackVftable::Method10)customAttackMethod10,
    (game::IBatAttackVftable::UnknownMethod)customAttackMethod11,
    (game::IBatAttackVftable::UnknownMethod)customAttackMethod12,
    (game::IBatAttackVftable::UnknownMethod)customAttackMethod13,
    (game::IBatAttackVftable::UnknownMethod)customAttackMethod14
};
// clang-format on

game::IBatAttack* customAttackCtor(CustomAttack* attack,
                                   game::IMidgardObjectMap* objectMap,
                                   const game::CMidgardID* id1,
                                   const game::CMidgardID* id2,
                                   int attackNumber)
{
    logDebug("newAttackType.log", "CustomAttack c-tor");
    attack->id1 = *id1;
    attack->id2 = *id2;
    attack->attackNumber = attackNumber;
    attack->attack = game::gameFunctions().getAttackByIdAndCheckTransfomed(objectMap, id1,
                                                                           attackNumber);
    attack->vftable = &customAttackVftable;
    return attack;
}

} // namespace hooks
