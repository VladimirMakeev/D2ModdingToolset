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

#ifndef UNITUTILS_H
#define UNITUTILS_H

#include "d2list.h"

namespace game {
struct CMidgardID;
struct CMidUnit;
struct IUsUnit;
struct IUsSoldier;
struct TUsUnitImpl;
struct TUsSoldierImpl;
struct LImmuneCat;
struct LAttackSource;
struct IAttack;
struct IMidgardObjectMap;
struct BattleMsgData;
struct CDynUpgrade;
struct IUsUnitExtension;
struct TypeDescriptor;
struct LLeaderAbility;
struct CMidPlayer;
} // namespace game

namespace hooks {

void generateUnitImplByAttackId(const game::CMidgardID* attackId);
game::IUsSoldier* castUnitImplToSoldierWithLogging(const game::IUsUnit* unitImpl);
void getSoldierAttackSourceImmunities(const game::LImmuneCat* immuneCat,
                                      const game::IUsSoldier* soldier,
                                      game::List<game::LAttackSource>* value);
bool isUnitSmall(const game::CMidUnit* unit);
game::CMidgardID getGlobalUnitImplId(const game::CMidgardID* unitImplId);
game::TUsUnitImpl* getGlobalUnitImpl(const game::CMidgardID* unitImplId);
game::TUsUnitImpl* generateUnitImpl(const game::CMidgardID* unitImplId, int level);
game::TUsUnitImpl* getUnitImpl(const game::CMidgardID* unitImplId);
game::TUsUnitImpl* getUnitImpl(const game::IUsUnit* unit);
game::TUsSoldierImpl* getSoldierImpl(const game::IUsUnit* unit);
game::TUsSoldierImpl* getSoldierImpl(const game::IUsSoldier* soldier);
game::IAttack* getAttack(const game::IUsUnit* unit, bool primary, bool checkAltAttack);
game::IAttack* getAltAttack(const game::IUsUnit* unit, bool primary);
int getArmor(const game::CMidgardID* unitId,
             const game::IUsSoldier* soldier,
             const game::BattleMsgData* battleMsgData,
             bool includeShattered,
             bool includeFortification);
const game::CDynUpgrade* getDynUpgrade(const game::IUsUnit* unit, int upgradeNumber);

/** Returns unit bonus armor from city, if he is inside one. */
int getCityProtection(const game::IMidgardObjectMap* objectMap, const game::CMidgardID* unitId);

/** Returns unit including bonus regen from lord, fort/ruin and terrain. */
int getUnitRegen(const game::IMidgardObjectMap* objectMap, const game::CMidgardID* unitId);

int getUnitHpMax(const game::CMidUnit* unit);

const char* getUnitName(const game::CMidUnit* unit);

/** Analogue of game::CastUnitImplTo* functions. */
game::IUsUnitExtension* castUnitImpl(const game::IUsUnit* unitImpl,
                                     const game::TypeDescriptor* type);

int computeUnitEffectiveHpForAi(int hp, int armor);
int computeUnitEffectiveHp(int hp, int armor);
int computeShatterDamage(const game::CMidgardID* unitId,
                         const game::IUsSoldier* soldier,
                         const game::BattleMsgData* battleMsgData,
                         const game::IAttack* attack);
void updateAttackCountAfterTransformation(game::BattleMsgData* battleMsgData,
                                          const game::CMidUnit* unit,
                                          bool prevAttackTwice);
bool isStackLeaderAndAllowedToUseBattleItems(const game::IMidgardObjectMap* objectMap,
                                             const game::CMidgardID* unitId,
                                             const game::BattleMsgData* battleMsgData);
bool hasLeaderAbility(const game::IUsUnit* unitImpl, const game::LLeaderAbility* ability);
bool hasCriticalHitLeaderAbility(const game::IUsUnit* unitImpl);

/** Returns true if the unit is changed during validation. */
bool validateUnit(game::CMidUnit* unit);

bool canUnitGainXp(const game::IUsUnit* unitImpl);

bool isNextUnitImpl(const game::IUsUnit* unitImpl, const game::IUsUnit* prevImpl);

bool hasNextTierUnitImpl(const game::IUsUnit* unitImpl);

bool hasMaxTierUpgradeBuilding(const game::IMidgardObjectMap* objectMap,
                               const game::IUsUnit* unitImpl);

bool requiresUpgradeBuilding(const game::IMidgardObjectMap* objectMap,
                             const game::CMidPlayer* player,
                             const game::IUsUnit* unitImpl);

const game::TUsUnitImpl* __stdcall getUpgradeUnitImpl(const game::IMidgardObjectMap* objectMap,
                                                      const game::CMidPlayer* player,
                                                      const game::CMidUnit* unit);

int getGeneratedUnitImplLevelMax();

} // namespace hooks

#endif // UNITUTILS_H
