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

#include "idlist.h"
#include "targetslist.h"

namespace game {
struct LAttackClass;
struct LAttackClassTable;
struct LAttackSource;
struct LAttackSourceTable;
struct LAttackReach;
struct LAttackReachTable;
struct LImmuneCat;
struct CMidUnit;
struct CMidStack;
struct CAttackImpl;
struct CAttackData;
struct IUsSoldier;
struct CDBTable;
struct GlobalData;
struct IAttack;
struct IBatAttack;
struct IMidgardObjectMap;
struct BattleMsgData;
struct CMidgardID;
struct CMidUnitGroup;
struct IItem;
struct CMqPoint;

enum class BattleAction : int;
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

game::LAttackReachTable* __fastcall attackReachTableCtorHooked(game::LAttackReachTable* thisptr,
                                                               int /*%edx*/,
                                                               const char* globalsFolderPath,
                                                               void* codeBaseEnvProxy);

game::CAttackImpl* __fastcall attackImplCtorHooked(game::CAttackImpl* thisptr,
                                                   int /*%edx*/,
                                                   const game::CDBTable* dbTable,
                                                   const game::GlobalData** globalData);

game::CAttackImpl* __fastcall attackImplCtor2Hooked(game::CAttackImpl* thisptr,
                                                    int /*%edx*/,
                                                    const game::CAttackData* data);

void __fastcall attackImplGetDataHooked(game::CAttackImpl* thisptr,
                                        int /*%edx*/,
                                        game::CAttackData* value);

game::IBatAttack* __stdcall createBatAttackHooked(game::IMidgardObjectMap* objectMap,
                                                  game::BattleMsgData* battleMsgData,
                                                  const game::CMidgardID* id1,
                                                  const game::CMidgardID* id2,
                                                  int attackNumber,
                                                  const game::LAttackClass* attackClass,
                                                  bool a7);

std::uint32_t __stdcall getAttackClassWardFlagPositionHooked(const game::LAttackClass* attackClass);

const char* __stdcall attackClassToStringHooked(const game::LAttackClass* attackClass);

void __stdcall getUnitAttackSourceImmunitiesHooked(const game::LImmuneCat* immuneCat,
                                                   const game::CMidUnit* unit,
                                                   game::List<game::LAttackSource>* value);

void __stdcall getSoldierAttackSourceImmunitiesHooked(const game::IUsSoldier* soldier,
                                                      bool alwaysImmune,
                                                      game::List<game::LAttackSource>* value);

double __stdcall getSoldierImmunityAiRatingHooked(const game::IUsSoldier* soldier);

double __stdcall getAttackClassAiRatingHooked(const game::IUsSoldier* soldier, bool a2);

double __stdcall getAttackReachAiRatingHooked(const game::IUsSoldier* soldier, int targetCount);

std::uint32_t __stdcall getAttackSourceWardFlagPositionHooked(
    const game::LAttackSource* attackSource);

bool __fastcall isUnitAttackSourceWardRemovedHooked(game::BattleMsgData* thisptr,
                                                    int /*%edx*/,
                                                    const game::CMidgardID* unitId,
                                                    const game::LAttackSource* attackSource);

void __fastcall removeUnitAttackSourceWardHooked(game::BattleMsgData* thisptr,
                                                 int /*%edx*/,
                                                 const game::CMidgardID* unitId,
                                                 const game::LAttackSource* attackSource);

void __stdcall addUnitToBattleMsgDataHooked(const game::IMidgardObjectMap* objectMap,
                                            const game::CMidUnitGroup* group,
                                            const game::CMidgardID* unitId,
                                            char attackerFlags,
                                            game::BattleMsgData* battleMsgData);

void __stdcall getTargetsToAttackHooked(game::IdList* value,
                                        const game::IMidgardObjectMap* objectMap,
                                        const game::IAttack* attack,
                                        const game::IBatAttack* batAttack,
                                        const game::LAttackReach* attackReach,
                                        const game::BattleMsgData* battleMsgData,
                                        game::BattleAction action,
                                        const game::CMidgardID* targetUnitId);

void __stdcall fillTargetsListHooked(const game::IMidgardObjectMap* objectMap,
                                     const game::BattleMsgData* battleMsgData,
                                     const game::IBatAttack* batAttack,
                                     const game::CMidgardID* unitId,
                                     const game::CMidgardID* attackUnitOrItemId,
                                     bool targetAllies,
                                     game::TargetsList* value,
                                     bool checkAltAttack);

void __stdcall fillEmptyTargetsListHooked(const game::IMidgardObjectMap* objectMap,
                                          const game::BattleMsgData* battleMsgData,
                                          const game::IBatAttack* batAttack,
                                          const game::CMidgardID* unitId,
                                          const game::CMidgardID* attackUnitOrItemId,
                                          bool targetAllies,
                                          game::TargetsList* value);

bool __stdcall isGroupSuitableForAiNobleMisfitHooked(const game::IMidgardObjectMap* objectMap,
                                                     const game::CMidUnitGroup* group);

bool __stdcall isUnitSuitableForAiNobleDuelHooked(const game::IMidgardObjectMap* objectMap,
                                                  const game::CMidgardID* unitId);

bool __stdcall findAttackTargetHooked(const game::IMidgardObjectMap* objectMap,
                                      const game::CMidgardID* unitId,
                                      const game::IAttack* attack,
                                      const game::CMidUnitGroup* targetGroup,
                                      const game::TargetsList* targets,
                                      const game::BattleMsgData* battleMsgData,
                                      game::CMidgardID* value);

bool __stdcall findDoppelgangerAttackTargetHooked(const game::IMidgardObjectMap* objectMap,
                                                  const game::CMidgardID* unitId,
                                                  const game::BattleMsgData* battleMsgData,
                                                  const game::CMidUnitGroup* targetGroup,
                                                  const game::TargetsList* targets,
                                                  game::CMidgardID* value);

bool __stdcall findDamageAttackTargetWithNonAllReachHooked(const game::IMidgardObjectMap* objectMap,
                                                           const game::IAttack* attack,
                                                           int damage,
                                                           const game::CMidUnitGroup* targetGroup,
                                                           const game::TargetsList* targets,
                                                           const game::BattleMsgData* battleMsgData,
                                                           game::CMidgardID* value);

bool __stdcall isAttackBetterThanItemUsageHooked(const game::IItem* item,
                                                 const game::IUsSoldier* soldier,
                                                 const game::CMidgardID* unitImplId);

game::CMidgardID* __stdcall getSummonUnitImplIdByAttackHooked(game::CMidgardID* summonImplId,
                                                              const game::CMidgardID* attackId,
                                                              int position,
                                                              bool smallUnit);

bool __stdcall isSmallMeleeFighterHooked(const game::IUsSoldier* soldier);

bool __stdcall cAiHireUnitEvalHooked(const game::IMidgardObjectMap* objectMap,
                                     const game::CMidUnitGroup* group);

double __stdcall getMeleeUnitToHireAiRatingHooked(const game::CMidgardID* unitImplId,
                                                  bool a2,
                                                  bool a3);

int __stdcall computeTargetUnitAiPriorityHooked(const game::IMidgardObjectMap* objectMap,
                                                const game::CMidgardID* unitId,
                                                const game::BattleMsgData* battleMsgData,
                                                int attackerDamage);

bool __fastcall midStackInitializeHooked(game::CMidStack* thisptr,
                                         int /*%edx*/,
                                         const game::IMidgardObjectMap* objectMap,
                                         const game::CMidgardID* leaderId,
                                         const game::CMidgardID* ownerId,
                                         const game::CMidgardID* subraceId,
                                         const game::CMqPoint* position);

} // namespace hooks

#endif // CUSTOMATTACKHOOKS_H
