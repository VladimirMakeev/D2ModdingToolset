/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#include "groupupgradehooks.h"
#include "battleattackinfo.h"
#include "battlemsgdata.h"
#include "battleutils.h"
#include "game.h"
#include "gameutils.h"
#include "midunit.h"
#include "usunitimpl.h"
#include "visitors.h"

namespace hooks {

bool upgradeUnit(game::IMidgardObjectMap* objectMap,
                 game::CMidUnit* unit,
                 const game::CMidgardID* groupId,
                 game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    auto upgradeUnitImpl = gameFunctions().getUpgradeUnitImplCheckXp(objectMap, unit);
    if (!upgradeUnitImpl) {
        return false;
    }

    BattleAttackUnitInfo info{};
    info.unitId = unit->id;
    info.unitImplId = unit->unitImpl->id;
    BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);

    VisitorApi::get().upgradeUnit(&unit->id, &upgradeUnitImpl->id, groupId, objectMap, 1);
    return true;
}

bool changeUnitXpTryUpgrade(game::IMidgardObjectMap* objectMap,
                            game::BattleMsgData* battleMsgData,
                            game::UnitInfo* unitInfo,
                            const game::CMidgardID* groupId,
                            const game::CMidgardID* playerId,
                            int unitXp,
                            game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battleApi = BattleMsgDataApi::get();

    if (!unitXp) {
        return false;
    }

    const auto unitId = unitInfo->unitId1;
    if (battleApi.getUnitStatus(battleMsgData, &unitId, BattleStatus::Dead)
        || battleApi.getUnitStatus(battleMsgData, &unitId, BattleStatus::Hidden)
        || battleApi.getUnitStatus(battleMsgData, &unitId, BattleStatus::Unsummoned)
        || battleApi.getUnitStatus(battleMsgData, &unitId, BattleStatus::Summon)) {
        return false;
    }

    if (fn.isUnitTierMax(objectMap, playerId, &unitId)
        && !fn.isUnitLevelNotMax(objectMap, playerId, &unitId)) {
        return false;
    }

    auto unit = fn.findUnitById(objectMap, &unitId);
    int xpReceived = adjustUnitXpReceived(battleMsgData, unit, unitXp);
    if (xpReceived <= 0) {
        return false;
    }

    auto previousXp = unit->currentXp;
    fn.changeUnitXpCheckUpgrade(objectMap, playerId, &unitId, xpReceived);
    xpReceived = unit->currentXp - previousXp;

    if (upgradeUnit(objectMap, unit, groupId, attackInfo)) {
        unitInfo->unitHp = unit->currentHp;
    }
    unitInfo->unitXp = xpReceived;
    return true;
}

int getXpPercent(game::IMidgardObjectMap* objectMap,
                 const game::CMidgardID* groupId,
                 const game::CMidgardID* playerId,
                 bool playableRaceAi)
{
    if (!canGroupGainXp(objectMap, groupId)) {
        return 0;
    }

    // Yes, xp bonuses are multiplicative
    int result = 100 + getWeaponMasterBonusXpPercent(objectMap, groupId);
    result += result * getEasyDifficultyBonusXpPercent(objectMap, playerId) / 100;
    if (playableRaceAi) {
        result += result * getAiBonusXpPercent(objectMap) / 100;
    }

    return result;
}

void __stdcall upgradeGroupHooked(game::IMidgardObjectMap* objectMap,
                                  game::BattleMsgData* battleMsgData,
                                  const game::CMidgardID* groupId,
                                  game::BattleAttackInfo** attackInfo,
                                  bool playableRaceAi)
{
    using namespace game;

    bool isAttacker = battleMsgData->attackerGroupId == *groupId;
    auto playerId = isAttacker ? battleMsgData->attackerPlayerId : battleMsgData->defenderPlayerId;
    int xpPercent = getXpPercent(objectMap, groupId, &playerId, playableRaceAi);

    for (auto& unitInfo : battleMsgData->unitsInfo) {
        if (unitInfo.unitId1 != invalidId && !unitInfo.unknown2
            && unitInfo.unitFlags.parts.attacker == isAttacker) {
            if (!changeUnitXpTryUpgrade(objectMap, battleMsgData, &unitInfo, groupId, &playerId,
                                        unitInfo.unitXp * xpPercent / 100, attackInfo)) {
                unitInfo.unitXp = 0;
            }
        }
    }
}

} // namespace hooks
