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

#ifndef CUSTOMATTACKUTILS_H
#define CUSTOMATTACKUTILS_H

#include "customattacks.h"
#include "idlist.h"
#include "targetset.h"
#include <filesystem>
#include <optional>

namespace game {
struct CMidgardID;
struct IMidgardObjectMap;
struct BattleMsgData;
struct IAttack;
struct IBatAttack;
} // namespace game

namespace bindings {
class ItemView;
class UnitSlotView;
class BattleMsgDataView;
} // namespace bindings

namespace hooks {

struct CustomAttackData;
struct CustomAttackReach;

using UnitSlots = std::vector<bindings::UnitSlotView>;

void fillCustomAttackSources(const std::filesystem::path& dbfFilePath);

void fillCustomAttackReaches(const std::filesystem::path& dbfFilePath);

UnitSlots getTargetsToSelectOrAttack(const std::string& scriptFile,
                                     const bindings::UnitSlotView& attacker,
                                     const bindings::UnitSlotView& selected,
                                     const UnitSlots& allies,
                                     const UnitSlots& targets,
                                     bool targetsAreAllies,
                                     const std::optional<bindings::ItemView>& item,
                                     const bindings::BattleMsgDataView& battle,
                                     bool isMarking);

UnitSlots getTargets(const game::IMidgardObjectMap* objectMap,
                     const game::BattleMsgData* battleMsgData,
                     const game::IBatAttack* batAttack,
                     const game::CMidgardID* targetGroupId,
                     const game::CMidgardID* unitId,
                     const game::CMidgardID* selectedUnitId);

UnitSlots getAllies(const game::IMidgardObjectMap* objectMap,
                    const game::BattleMsgData* battleMsgData,
                    const game::CMidgardID* unitGroupId,
                    const game::CMidgardID* unitId);

void fillTargetsListForCustomAttackReach(const game::IMidgardObjectMap* objectMap,
                                         const game::BattleMsgData* battleMsgData,
                                         const game::IBatAttack* batAttack,
                                         const game::CMidgardID* targetGroupId,
                                         const game::CMidgardID* unitGroupId,
                                         const game::CMidgardID* unitId,
                                         const game::CMidgardID* attackUnitOrItemId,
                                         const CustomAttackReach& attackReach,
                                         game::TargetSet* value);

void getTargetsToAttackForAllAttackReach(const game::IMidgardObjectMap* objectMap,
                                         const game::BattleMsgData* battleMsgData,
                                         const game::IAttack* attack,
                                         const game::IBatAttack* batAttack,
                                         const game::CMidgardID* targetGroupId,
                                         const game::CMidgardID* targetUnitId,
                                         game::IdList* value);

UnitSlots getTargetsToMarkOrAttackForCustomAttackReach(const game::IMidgardObjectMap* objectMap,
                                                       const game::BattleMsgData* battleMsgData,
                                                       const game::IBatAttack* batAttack,
                                                       const game::CMidgardID* targetGroupId,
                                                       const game::CMidgardID* targetUnitId,
                                                       const game::CMidgardID* unitGroupId,
                                                       const game::CMidgardID* unitId,
                                                       const CustomAttackReach& attackReach,
                                                       bool isMarking);

UnitSlots getTargetsToAttackForCustomAttackReach(const game::IMidgardObjectMap* objectMap,
                                                 const game::BattleMsgData* battleMsgData,
                                                 const game::IBatAttack* batAttack,
                                                 const game::CMidgardID* targetGroupId,
                                                 const game::CMidgardID* targetUnitId,
                                                 const game::CMidgardID* unitGroupId,
                                                 const game::CMidgardID* unitId,
                                                 const CustomAttackReach& attackReach);

UnitSlots getTargetsToMarkForCustomAttackReach(const game::IMidgardObjectMap* objectMap,
                                               const game::BattleMsgData* battleMsgData,
                                               const game::IAttack* attack,
                                               const game::CMidgardID* targetGroupId,
                                               const game::CMidgardID* targetUnitId,
                                               const game::CMidgardID* unitGroupId,
                                               const game::CMidgardID* unitId,
                                               const game::CMidgardID* attackUnitId,
                                               const CustomAttackReach& attackReach);

void getTargetsToAttackForCustomAttackReach(const game::IMidgardObjectMap* objectMap,
                                            const game::BattleMsgData* battleMsgData,
                                            const game::IBatAttack* batAttack,
                                            const game::CMidgardID* targetGroupId,
                                            const game::CMidgardID* targetUnitId,
                                            const game::CMidgardID* unitGroupId,
                                            const game::CMidgardID* unitId,
                                            const CustomAttackReach& attackReach,
                                            game::IdList* value);

bool shouldExcludeImmuneTargets(const game::IMidgardObjectMap* objectMap,
                                const game::BattleMsgData* battleMsgData,
                                const game::CMidgardID* unitId);

void excludeImmuneTargets(const game::IMidgardObjectMap* objectMap,
                          const game::BattleMsgData* battleMsgData,
                          const game::IAttack* attack,
                          const game::CMidgardID* unitGroupId,
                          const game::CMidgardID* targetGroupId,
                          game::TargetSet* value);

void fillCustomAttackTargets(const game::IdList* targets);

const CustomAttackDamageRatios& getCustomDamageRatios(const game::IAttack* attack);

int applyAttackDamageRatio(int damage, double ratio);

std::vector<double> computeAttackDamageRatio(const CustomAttackData& customData, int targetCount);

double computeTotalDamageRatio(const game::IAttack* attack, int targetCount);

int computeAverageTotalDamage(const game::IAttack* attack, int damage);

CustomAttackData getCustomAttackData(const game::IAttack* attack);

} // namespace hooks

#endif // CUSTOMATTACKUTILS_H
