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

#ifndef CUSTOMATTACKHOOKS_H
#define CUSTOMATTACKHOOKS_H

#include "linkedlist.h"

namespace game {
struct LAttackClass;
struct LAttackClassTable;
struct LAttackSource;
struct LAttackSourceTable;
struct LImmuneCat;
struct CMidUnit;
struct CAttackImpl;
struct IUsSoldier;
struct CDBTable;
struct GlobalData;
struct IBatAttack;
struct IMidgardObjectMap;
struct BattleMsgData;
struct CMidgardID;
} // namespace game

namespace hooks {

extern game::LAttackClass customAttackClass;

game::LAttackClassTable* __fastcall attackClassTableCtorHooked(game::LAttackClassTable* thisptr,
                                                               int /*%edx*/,
                                                               const char* globalsFolderPath,
                                                               void* codeBaseEnvProxy);

game::LAttackSourceTable* __fastcall attackSourceTableCtorHooked(game::LAttackSourceTable* thisptr,
                                                                 int /*%edx*/,
                                                                 const char* globalsFolderPath,
                                                                 void* codeBaseEnvProxy);

game::CAttackImpl* __fastcall attackImplCtorHooked(game::CAttackImpl* thisptr,
                                                   int /*%edx*/,
                                                   const game::CDBTable* dbTable,
                                                   const game::GlobalData** globalData);

game::IBatAttack* __stdcall createBatAttackHooked(game::IMidgardObjectMap* objectMap,
                                                  game::BattleMsgData* battleMsgData,
                                                  const game::CMidgardID* id1,
                                                  const game::CMidgardID* id2,
                                                  int attackNumber,
                                                  const game::LAttackClass* attackClass,
                                                  bool a7);

int __stdcall getAttackClassWardFlagPositionHooked(const game::LAttackClass* attackClass);

const char* __stdcall attackClassToStringHooked(const game::LAttackClass* attackClass);

void __stdcall getUnitAttackSourceImmunitiesHooked(const game::LImmuneCat* immuneCat,
                                                   const game::CMidUnit* unit,
                                                   game::LinkedList<game::LAttackSource>* value);

void __stdcall getSoldierAttackSourceImmunitiesHooked(const game::IUsSoldier* soldier,
                                                      bool alwaysImmune,
                                                      game::LinkedList<game::LAttackSource>* value);

double __stdcall getSoldierImmunityPowerHooked(const game::IUsSoldier* soldier);

int __stdcall getAttackSourceWardFlagPositionHooked(const game::LAttackSource* attackSource);

bool __fastcall isUnitAttackSourceWardRemovedHooked(game::BattleMsgData* thisptr,
                                                    int /*%edx*/,
                                                    const game::CMidgardID* unitId,
                                                    const game::LAttackSource* attackSource);

void __fastcall removeUnitAttackSourceWardHooked(game::BattleMsgData* thisptr,
                                                 int /*%edx*/,
                                                 const game::CMidgardID* unitId,
                                                 const game::LAttackSource* attackSource);

} // namespace hooks

#endif // CUSTOMATTACKHOOKS_H
