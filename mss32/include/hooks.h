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

#ifndef HOOKS_H
#define HOOKS_H

#include "attack.h"
#include "idlist.h"
#include "targetslist.h"
#include <string>
#include <utility>
#include <vector>

namespace game {
struct AutoDialogData;
struct CMenuNewSkirmishSingle;
struct CMidDataCache2;
struct GlobalData;
struct CDBTable;
struct LBuildingCategoryTable;
struct CBuildingBranch;
struct IUsSoldier;
struct IMidgardObjectMap;
struct BattleMsgData;
struct CBatAttackGiveAttack;
struct CBatAttackShatter;
struct BattleAttackInfo;
struct CMidPlayer;
struct CEncLayoutSpell;
struct CInterface;
struct CEncParamBase;
struct CDDCarryOverItems;
struct IMidDropManager;
struct CListBoxInterf;
struct CPhaseGame;
struct CMqPoint;
struct os_exception;
struct CMidUnit;
struct CMidUnitGroup;
struct CUnitGenerator;
struct IItem;
}; // namespace game

namespace hooks {

struct HookInfo
{
    void* target;
    void* hook;
    void** original;
};
using Hooks = std::vector<HookInfo>;

/** Returns array of hooks to setup. */
Hooks getHooks();
Hooks getVftableHooks();

void respopupInitHooked(void);
void* __fastcall toggleShowBannersInitHooked(void* thisptr, int /*%edx*/);

bool __fastcall removeModifierHooked(game::CMidUnit* thisptr,
                                     int /*%edx*/,
                                     const game::CMidgardID* modifierId);

game::AutoDialogData* __fastcall loadScriptFileHooked(game::AutoDialogData* thisptr,
                                                      int /*%edx*/,
                                                      const char* filePath,
                                                      int /*unknown*/);

game::CMenuNewSkirmishSingle* __fastcall menuNewSkirmishSingleCtorHooked(
    game::CMenuNewSkirmishSingle* thisptr,
    int /*%edx*/,
    int a1);

bool __stdcall addPlayerUnitsToHireListHooked(game::CMidDataCache2* dataCache,
                                              const game::CMidgardID* playerId,
                                              const game::CMidgardID* a3,
                                              game::IdList* hireList);

/**
 * Create TBuildingUnitUpgType objects for buildings from heal category.
 * This allows to assign temples as buildings required to hire units.
 */
void __stdcall createBuildingTypeHooked(const game::CDBTable* dbTable,
                                        void* a2,
                                        const game::GlobalData** globalData);

game::LBuildingCategoryTable* __fastcall buildingCategoryTableCtorHooked(
    game::LBuildingCategoryTable* thisptr,
    int /*%edx*/,
    const char* globalsFolderPath,
    void* codeBaseEnvProxy);

game::CBuildingBranch* __fastcall buildingBranchCtorHooked(game::CBuildingBranch* thisptr,
                                                           int /*%edx*/,
                                                           game::CPhaseGame* phaseGame,
                                                           int* branchNumber);

int __stdcall chooseUnitLaneHooked(const game::IUsSoldier* soldier);

bool __stdcall isTurnValidHooked(int turn);

game::CMidgardID* __stdcall radioButtonIndexToPlayerIdHooked(game::CMidgardID* playerId,
                                                             game::IMidgardObjectMap* objectMap,
                                                             int index);

bool __fastcall giveAttackCanPerformHooked(game::CBatAttackGiveAttack* thisptr,
                                           int /*%edx*/,
                                           game::IMidgardObjectMap* objectMap,
                                           game::BattleMsgData* battleMsgData,
                                           game::CMidgardID* unitId);

bool __fastcall shatterCanPerformHooked(game::CBatAttackShatter* thisptr,
                                        int /*%edx*/,
                                        game::IMidgardObjectMap* objectMap,
                                        game::BattleMsgData* battleMsgData,
                                        game::CMidgardID* unitId);

void __fastcall setUnitShatteredArmorHooked(game::BattleMsgData* thisptr,
                                            int /*%edx*/,
                                            const game::CMidgardID* unitId,
                                            int armor);

void __fastcall shatterOnHitHooked(game::CBatAttackShatter* thisptr,
                                   int /*%edx*/,
                                   game::IMidgardObjectMap* objectMap,
                                   game::BattleMsgData* battleMsgData,
                                   game::CMidgardID* unitId,
                                   game::BattleAttackInfo** attackInfo);

bool __fastcall shatterCanMissHooked(game::CBatAttackShatter* thisptr,
                                     int /*%edx*/,
                                     game::BattleMsgData* battleMsgData,
                                     game::CMidgardID* id);

int __stdcall deletePlayerBuildingsHooked(game::IMidgardObjectMap* objectMap,
                                          game::CMidPlayer* player);

game::CEncLayoutSpell* __fastcall encLayoutSpellCtorHooked(game::CEncLayoutSpell* thisptr,
                                                           int /*%edx*/,
                                                           game::IMidgardObjectMap* objectMap,
                                                           game::CInterface* interface,
                                                           void* a2,
                                                           game::CMidgardID* spellId,
                                                           game::CEncParamBase* encParam,
                                                           game::CMidgardID* playerId);

int __stdcall countStacksOnMapHooked(game::IMidgardObjectMap* objectMap);

game::CDDCarryOverItems* __fastcall carryOverItemsCtorHooked(game::CDDCarryOverItems* thisptr,
                                                             int /*%edx*/,
                                                             game::IMidDropManager* dropManager,
                                                             game::CListBoxInterf* listBox,
                                                             game::CPhaseGame* phaseGame,
                                                             int carryOverItemsMax);

void __fastcall markMapPositionHooked(void* thisptr, int /*%edx*/, game::CMqPoint* position);

int __stdcall computeDamageHooked(const game::IMidgardObjectMap* objectMap,
                                  const game::BattleMsgData* battleMsgData,
                                  const game::IAttack* attack,
                                  const game::CMidgardID* attackerUnitId,
                                  const game::CMidgardID* targetUnitId,
                                  bool computeCriticalHit,
                                  int* attackDamage,
                                  int* criticalHitDamage);

void __stdcall getAttackPowerHooked(int* power,
                                    const game::IAttack* attack,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* unitId,
                                    const game::BattleMsgData* battleMsgData);

bool __stdcall attackShouldMissHooked(const int* power);

int __stdcall getUnitHealAttackNumberHooked(const game::IMidgardObjectMap* objectMap,
                                            const game::CMidgardID* unitId);

int __stdcall getAttackQtyDamageOrHealHooked(const game::IAttack* attack, int damageMax);

void __stdcall setUnknown9Bit1AndClearBoostLowerDamageHooked(game::BattleMsgData* battleMsgData,
                                                             const game::CMidgardID* unitId,
                                                             game::CMidgardID* nextAttackUnitId);

void __stdcall beforeAttackHooked(game::BattleMsgData* battleMsgData,
                                  game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* unitId);

void __stdcall throwExceptionHooked(const game::os_exception* thisptr, const void* throwInfo);

int __stdcall computeUnitEffectiveHpHooked(const game::IMidgardObjectMap* objectMap,
                                           const game::CMidUnit* unit,
                                           const game::BattleMsgData* battleMsgData);

void __stdcall applyDynUpgradeToAttackDataHooked(const game::CMidgardID* unitImplId,
                                                 game::CUnitGenerator* unitGenerator,
                                                 int unitLevel,
                                                 game::IdType dynUpgradeType,
                                                 const game::CMidgardID* altAttackId,
                                                 game::CAttackData* attackData);

bool __stdcall findAttackTargetHooked(game::IMidgardObjectMap* objectMap,
                                      game::CMidgardID* unitId,
                                      game::IAttack* attack,
                                      game::CMidUnitGroup* targetGroup,
                                      game::TargetsList* targets,
                                      game::BattleMsgData* battleMsgData,
                                      game::CMidgardID* targetUnitId);

bool __stdcall findDoppelgangerAttackTargetHooked(game::IMidgardObjectMap* objectMap,
                                                  game::CMidgardID* unitId,
                                                  game::BattleMsgData* battleMsgData,
                                                  game::CMidUnitGroup* targetGroup,
                                                  game::TargetsList* targets,
                                                  game::CMidgardID* targetUnitId);

bool __stdcall isAttackBetterThanItemUsageHooked(game::IItem* item,
                                                 game::IUsSoldier* soldier,
                                                 game::CMidgardID* unitImplId);

} // namespace hooks

#endif // HOOKS_H
