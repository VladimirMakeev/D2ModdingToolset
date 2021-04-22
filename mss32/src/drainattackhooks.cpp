/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#include "drainattackhooks.h"
#include "attack.h"
#include "batattackdrain.h"
#include "batattackdrainoverflow.h"
#include "battleattackinfo.h"
#include "battlemsgdata.h"
#include "game.h"
#include "hooks.h"
#include "log.h"
#include "midgardobjectmap.h"
#include "midunit.h"
#include "settings.h"
#include "usunitimpl.h"
#include "utils.h"
#include "visitors.h"
#include <fmt/format.h>

namespace hooks {

static void addBattleAttackInfo(game::BattleAttackInfo* attackInfo,
                                const game::CMidUnit* unit,
                                int damage,
                                int criticalHitDamage = 0)
{
    using namespace game;

    BattleAttackUnitInfo info{};
    info.unitId = unit->unitId;
    info.unitImplId = unit->unitImpl->unitId;
    info.damage = damage;
    info.criticalDamage = criticalHitDamage;

    BattleAttackInfoApi::get().addUnitInfo(&attackInfo->unitsInfo, &info);
}

static int drainAttack(game::IMidgardObjectMap* objectMap,
                       game::BattleMsgData* battleMsgData,
                       game::IAttack* attack,
                       const game::CMidgardID* attackerUnitId,
                       const game::CMidgardID* targetUnitId,
                       game::BattleAttackInfo* attackInfo,
                       int drainHealPercent)
{
    using namespace game;

    const auto& fn = gameFunctions();

    if (fn.isUnitImmuneToAttack(objectMap, battleMsgData, targetUnitId, attack, false)) {
        return 0;
    }

    int attackDamage{};
    int criticalHitDamage{};
    int fullDamage = fn.computeDamage(objectMap, battleMsgData, attack, attackerUnitId,
                                      targetUnitId, true, &attackDamage, &criticalHitDamage);

    auto targetUnit = static_cast<const CMidUnit*>(
        objectMap->vftable->findScenarioObjectById(objectMap, targetUnitId));
    const auto targetInitialHp = targetUnit->currentHp;

    const auto& visitors = VisitorApi::get();
    visitors.changeUnitHp(targetUnitId, -fullDamage, objectMap, 1);

    const auto targetResultingHp = targetUnit->currentHp;
    auto drainDamage = (targetInitialHp - targetResultingHp) * drainHealPercent / 100;

    drainDamage += attack->vftable->getDrain(attack, targetInitialHp - targetResultingHp);

    const auto& battle = BattleMsgDataApi::get();
    battle.checkUnitDeath(objectMap, battleMsgData, targetUnitId);

    auto attackerUnit = static_cast<const CMidUnit*>(
        objectMap->vftable->findScenarioObjectById(objectMap, attackerUnitId));
    const auto attackerInitialHp = attackerUnit->currentHp;

    visitors.changeUnitHp(attackerUnitId, drainDamage, objectMap, 1);

    const auto hpDiff = targetInitialHp - targetResultingHp;
    int damage{};
    int criticalDamage{};

    if (hpDiff < criticalHitDamage) {
        criticalDamage = hpDiff;
    } else {
        damage = hpDiff - criticalHitDamage;
        criticalDamage = criticalHitDamage;
    }

    addBattleAttackInfo(attackInfo, targetUnit, damage, criticalDamage);

    const auto attackerHpDiff = attackerUnit->currentHp - attackerInitialHp;
    addBattleAttackInfo(attackInfo, attackerUnit, attackerHpDiff);

    battle.setUnitHp(battleMsgData, targetUnitId, targetUnit->currentHp);
    battle.setUnitHp(battleMsgData, attackerUnitId, attackerUnit->currentHp);

    // drain overflow
    return drainDamage - attackerHpDiff;
}

void __fastcall drainAttackOnHitHooked(game::CBatAttackDrain* thisptr,
                                       int /*%edx*/,
                                       game::IMidgardObjectMap* objectMap,
                                       game::BattleMsgData* battleMsgData,
                                       game::CMidgardID* unitId,
                                       game::BattleAttackInfo** attackInfo)
{
    drainAttack(objectMap, battleMsgData, thisptr->attack, &thisptr->unitId, unitId, *attackInfo,
                userSettings().drainAttackHeal);
}

void __fastcall drainOverflowAttackOnHitHooked(game::CBatAttackDrainOverflow* thisptr,
                                               int /*%edx*/,
                                               game::IMidgardObjectMap* objectMap,
                                               game::BattleMsgData* battleMsgData,
                                               game::CMidgardID* unitId,
                                               game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    const auto& fn = gameFunctions();

    const auto drainOverflow = drainAttack(objectMap, battleMsgData, thisptr->attack,
                                           &thisptr->unitId, unitId, *attackInfo,
                                           userSettings().drainOverflowHeal);
    if (!drainOverflow) {
        return;
    }

    CMidgardID alliedStackId{};
    fn.getAllyOrEnemyStackId(&alliedStackId, battleMsgData, &thisptr->unitId, true);

    const auto& attack = CBatAttackDrainOverflowApi::get();

    DrainOverflowHealData healData{};
    attack.healDataCtor(&healData);

    attack.computeDrainOverflowGroupHeal(&healData, objectMap, &alliedStackId, &thisptr->unitId,
                                         drainOverflow);

    DrainOverflowHealIterator tmpIterator{};
    attack.healDataIteratorCtor(&healData, &tmpIterator);

    DrainOverflowHealIterator iterator{};
    attack.healDataIteratorCopyCtor(&iterator, &tmpIterator);

    DrainOverflowHealIterator tmpEndIterator{};
    attack.healDataEndIteratorCtor(&healData, &tmpEndIterator);

    DrainOverflowHealIterator endIterator{};
    attack.healDataIteratorCopyCtor(&endIterator, &tmpEndIterator);

    const auto& visitors = VisitorApi::get();
    const auto& battle = BattleMsgDataApi::get();

    DrainOverflowHealIterator tmp{};
    while (attack.isHealDataIteratorAtEnd(&iterator, &endIterator)) {
        const void* data = attack.healDataIteratorGetData(&iterator);

        auto healingUnitId = static_cast<const CMidgardID*>(data);
        const auto unitHeal = *(static_cast<const int*>(data) + 1);
        auto unit = static_cast<const CMidUnit*>(
            objectMap->vftable->findScenarioObjectById(objectMap, healingUnitId));

        const auto unitInitialHp = unit->currentHp;
        visitors.changeUnitHp(healingUnitId, unitHeal, objectMap, 1);
        const auto unitResultingHp = unit->currentHp;

        addBattleAttackInfo(*attackInfo, unit, unitResultingHp - unitInitialHp);
        battle.setUnitHp(battleMsgData, healingUnitId, unitResultingHp);

        attack.healDataIteratorAdvance(&iterator, &tmp, 0);
    }

    attack.healDataDtor(&healData);
}

} // namespace hooks
