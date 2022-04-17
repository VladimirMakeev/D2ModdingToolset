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

#include "battlemsgdatahooks.h"
#include "batattack.h"
#include "customattacks.h"
#include "gameutils.h"
#include "log.h"
#include "midstack.h"
#include "modifierutils.h"
#include "originalfunctions.h"
#include "unitutils.h"
#include <atomic>
#include <fmt/format.h>

namespace hooks {

class ModifiedUnitsPatchedFactory
{
public:
    ModifiedUnitsPatchedFactory()
        : count(0)
    { }

    ~ModifiedUnitsPatchedFactory()
    {
        if (count != 0) {
            logError("mssProxyError.log",
                     fmt::format("{:d} instances of ModifiedUnitsPatched remained on finalization",
                                 count));
        }
    }

    game::ModifiedUnitInfo* create()
    {
        count++;
        return new game::ModifiedUnitInfo[game::ModifiedUnitCountPatched];
    }

    void destroy(game::ModifiedUnitInfo* value)
    {
        count--;
        delete[] value;
    }

private:
    std::atomic<int> count;
} modifiedUnitsPatchedFactory;

void resetUnitInfo(game::UnitInfo* unitInfo)
{
    using namespace game;

    auto modifiedUnits = unitInfo->modifiedUnits;
    memset(unitInfo, 0, sizeof(UnitInfo));

    unitInfo->unitId1 = invalidId;

    unitInfo->modifiedUnits = modifiedUnits;
    resetModifiedUnitsInfo(unitInfo);

    for (auto& modifierId : unitInfo->modifierIds) {
        modifierId = invalidId;
    }
}

game::BattleMsgData* __fastcall battleMsgDataCtorHooked(game::BattleMsgData* thisptr, int /*%edx*/)
{
    using namespace game;

    getOriginalFunctions().battleMsgDataCtor(thisptr);

    for (auto& unitInfo : thisptr->unitsInfo) {
        memset(&unitInfo.modifiedUnits, 0, sizeof(ModifiedUnitsPatched));
        unitInfo.modifiedUnits.patched = modifiedUnitsPatchedFactory.create();
        resetModifiedUnitsInfo(&unitInfo);
    }

    return thisptr;
}

game::BattleMsgData* __fastcall battleMsgDataCopyCtorHooked(game::BattleMsgData* thisptr,
                                                            int /*%edx*/,
                                                            const game::BattleMsgData* src)
{
    using namespace game;

    getOriginalFunctions().battleMsgDataCtor(thisptr);

    for (auto& unitInfo : thisptr->unitsInfo) {
        memset(&unitInfo.modifiedUnits, 0, sizeof(ModifiedUnitsPatched));
    }

    return battleMsgDataCopyHooked(thisptr, 0, src);
}

game::BattleMsgData* __fastcall battleMsgDataCopyAssignHooked(game::BattleMsgData* thisptr,
                                                              int /*%edx*/,
                                                              const game::BattleMsgData* src)
{
    using namespace game;

    if (thisptr == src)
        return thisptr;

    const size_t count = std::size(thisptr->unitsInfo);
    std::vector<ModifiedUnitInfo*> prev(count);
    for (size_t i = 0; i < count; i++) {
        prev[i] = thisptr->unitsInfo[i].modifiedUnits.patched;
    }

    *thisptr = *src;

    for (size_t i = 0; i < count; i++) {
        memcpy(prev[i], src->unitsInfo[i].modifiedUnits.patched,
               sizeof(ModifiedUnitInfo) * ModifiedUnitCountPatched);
        thisptr->unitsInfo[i].modifiedUnits.patched = prev[i];
    }

    return thisptr;
}

game::BattleMsgData* __fastcall battleMsgDataCopyHooked(game::BattleMsgData* thisptr,
                                                        int /*%edx*/,
                                                        const game::BattleMsgData* src)
{
    using namespace game;

    *thisptr = *src;

    const size_t count = std::size(thisptr->unitsInfo);
    for (size_t i = 0; i < count; i++) {
        auto modifiedUnits = modifiedUnitsPatchedFactory.create();
        memcpy(modifiedUnits, src->unitsInfo[i].modifiedUnits.patched,
               sizeof(ModifiedUnitInfo) * ModifiedUnitCountPatched);
        thisptr->unitsInfo[i].modifiedUnits.patched = modifiedUnits;
    }

    return thisptr;
}

void __fastcall battleMsgDataDtorHooked(game::BattleMsgData* thisptr, int /*%edx*/)
{
    using namespace game;

    for (auto& unitInfo : thisptr->unitsInfo) {
        modifiedUnitsPatchedFactory.destroy(unitInfo.modifiedUnits.patched);
        unitInfo.modifiedUnits.patched = nullptr;
    }
}

void __fastcall removeUnitInfoHooked(game::BattleMsgData* thisptr,
                                     int /*%edx*/,
                                     const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();

    size_t index;
    ModifiedUnitsPatched modifiedUnits{};
    const size_t count = std::size(thisptr->unitsInfo);
    for (index = 0; index < count; ++index) {
        if (thisptr->unitsInfo[index].unitId1 == *unitId) {
            modifiedUnits = thisptr->unitsInfo[index].modifiedUnits;
            break;
        }
    }

    for (size_t i = index; i + 1 < count; ++i) {
        memcpy(&thisptr->unitsInfo[i], &thisptr->unitsInfo[i + 1], sizeof(UnitInfo));
    }

    if (index < count) {
        auto lastInfo = &thisptr->unitsInfo[count - 1];
        lastInfo->modifiedUnits = modifiedUnits;
        resetUnitInfo(lastInfo);
    }

    while (battle.decreaseUnitAttacks(thisptr, unitId))
        ;
}

void updateDefendBattleAction(const game::UnitInfo* unitInfo,
                              game::Set<game::BattleAction>* actions)
{
    using namespace game;

    const auto& targetSetApi = TargetSetApi::get();

    if (unitInfo->unitFlags.parts.attackedOnceOfTwice)
        return;

    BattleAction defend = BattleAction::Defend;
    Pair<TargetSetIterator, bool> tmp{};
    targetSetApi.insert((TargetSet*)actions, &tmp, (int*)&defend);
}

void updateWaitBattleAction(const game::BattleMsgData* battleMsgData,
                            const game::UnitInfo* unitInfo,
                            game::Set<game::BattleAction>* actions)
{
    using namespace game;

    const auto& targetSetApi = TargetSetApi::get();

    if (battleMsgData->currentRound < 1)
        return;

    if (unitInfo->unitFlags.parts.waited || unitInfo->unitFlags.parts.attackedOnceOfTwice)
        return;

    BattleAction wait = BattleAction::Wait;
    Pair<TargetSetIterator, bool> tmp{};
    targetSetApi.insert((TargetSet*)actions, &tmp, (int*)&wait);
}

void updateUseItemBattleAction(const game::IMidgardObjectMap* objectMap,
                               const game::BattleMsgData* battleMsgData,
                               const game::UnitInfo* unitInfo,
                               game::Set<game::BattleAction>* actions,
                               game::GroupIdTargetsPair* item1Targets,
                               game::GroupIdTargetsPair* item2Targets)
{
    using namespace game;

    const auto& idApi = CMidgardIDApi::get();
    const auto& battleApi = BattleMsgDataApi::get();
    const auto& targetSetApi = TargetSetApi::get();

    if (battleMsgData->currentRound < 1)
        return;

    auto groupId = unitInfo->unitFlags.parts.attacker ? &battleMsgData->attackerGroupId
                                                      : &battleMsgData->defenderGroupId;
    if (idApi.getType(groupId) != IdType::Stack)
        return;

    if (!isStackLeaderAndAllowedToUseBattleItems(objectMap, &unitInfo->unitId1, battleMsgData))
        return;

    auto stack = getStack(objectMap, groupId);

    GroupIdTargetsPair* itemTargets[] = {item1Targets, item2Targets};
    EquippedItemIdx itemIndices[] = {EquippedItemIdx::Battle1, EquippedItemIdx::Battle2};
    for (int i = 0; i < 2; ++i) {
        auto itemId = stack->leaderEquippedItems.bgn[itemIndices[i]];
        if (itemId == emptyId)
            continue;

        auto& inventory = stack->inventory;
        if (inventory.vftable->getItemIndex(&inventory, &itemId) == -1)
            continue;

        bool used = false;
        for (const auto& usedItemId : battleMsgData->usedItemIds) {
            if (usedItemId == itemId) {
                used = true;
                break;
            }
        }
        if (used)
            continue;

        battleApi.getItemAttackTargets(objectMap, battleMsgData, &unitInfo->unitId1, &itemId,
                                       itemTargets[i]);
    }

    for (const auto targets : itemTargets) {
        if (targets->second.length > 0) {
            BattleAction useItem = BattleAction::UseItem;
            Pair<TargetSetIterator, bool> tmp{};
            targetSetApi.insert((TargetSet*)actions, &tmp, (int*)&useItem);
            break;
        }
    }
}

void updateRetreatBattleAction(const game::IMidgardObjectMap* objectMap,
                               const game::BattleMsgData* battleMsgData,
                               const game::UnitInfo* unitInfo,
                               game::Set<game::BattleAction>* actions)
{
    using namespace game;

    const auto& idApi = CMidgardIDApi::get();
    const auto& battleApi = BattleMsgDataApi::get();
    const auto& targetSetApi = TargetSetApi::get();

    if (battleMsgData->currentRound < 1)
        return;

    if (unitInfo->unitFlags.parts.attackedOnceOfTwice)
        return;

    auto groupId = unitInfo->unitFlags.parts.attacker ? &battleMsgData->attackerGroupId
                                                      : &battleMsgData->defenderGroupId;
    if (idApi.getType(groupId) != IdType::Stack)
        return;

    if (!unitInfo->unitFlags.parts.attacker) {
        auto stack = getStack(objectMap, groupId);
        if (stack && stack->insideId != emptyId)
            return;
    }

    BattleAction retreat = BattleAction::Retreat;
    Pair<TargetSetIterator, bool> tmp{};
    targetSetApi.insert((TargetSet*)actions, &tmp, (int*)&retreat);
}

void updateAttackBattleAction(const game::IMidgardObjectMap* objectMap,
                              const game::BattleMsgData* battleMsgData,
                              const game::UnitInfo* unitInfo,
                              game::Set<game::BattleAction>* actions,
                              game::GroupIdTargetsPair* attackTargets)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& targetSetApi = TargetSetApi::get();

    const auto attack = fn.getAttackById(objectMap, &unitInfo->unitId1, 1, false);
    const auto attackClass = attack->vftable->getAttackClass(attack);
    const auto batAttack = fn.createBatAttack(objectMap, battleMsgData, &unitInfo->unitId1,
                                              &unitInfo->unitId1, 1, attackClass, false);
    batAttack->vftable->getTargetGroupId(batAttack, &attackTargets->first, battleMsgData);
    batAttack->vftable->fillTargetsList(batAttack, objectMap, battleMsgData,
                                        &attackTargets->second);
    batAttack->vftable->destructor(batAttack, true);

    if (attackTargets->second.length > 0) {
        BattleAction attack = BattleAction::Attack;
        Pair<TargetSetIterator, bool> tmp{};
        targetSetApi.insert((TargetSet*)actions, &tmp, (int*)&attack);
    }
}

void __stdcall updateBattleActionsHooked(const game::IMidgardObjectMap* objectMap,
                                         const game::BattleMsgData* battleMsgData,
                                         const game::CMidgardID* unitId,
                                         game::Set<game::BattleAction>* actions,
                                         game::GroupIdTargetsPair* attackTargets,
                                         game::GroupIdTargetsPair* item1Targets,
                                         game::GroupIdTargetsPair* item2Targets)
{
    using namespace game;

    const auto& battleApi = BattleMsgDataApi::get();
    const auto& targetSetApi = TargetSetApi::get();

    targetSetApi.clear((TargetSet*)actions);

    auto unitInfo = battleApi.getUnitInfoById(battleMsgData, unitId);

    updateDefendBattleAction(unitInfo, actions);
    updateWaitBattleAction(battleMsgData, unitInfo, actions);
    updateUseItemBattleAction(objectMap, battleMsgData, unitInfo, actions, item1Targets,
                              item2Targets);
    updateRetreatBattleAction(objectMap, battleMsgData, unitInfo, actions);
    updateAttackBattleAction(objectMap, battleMsgData, unitInfo, actions, attackTargets);
}

void __fastcall beforeBattleRoundHooked(game::BattleMsgData* thisptr, int /*%edx*/)
{
    using namespace game;

    getOriginalFunctions().beforeBattleRound(thisptr);

    // Fix free transform-self to properly reset if the same unit has consequent turns in consequent
    // battles
    auto& freeTransformSelf = getCustomAttacks().freeTransformSelf;
    freeTransformSelf.unitId = emptyId;
    freeTransformSelf.turnCount = 0;
    freeTransformSelf.used = false;
}

} // namespace hooks
