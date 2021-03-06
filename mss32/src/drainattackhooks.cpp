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
#include "battleattackinfo.h"
#include "battlemsgdata.h"
#include "game.h"
#include "log.h"
#include "midgardobjectmap.h"
#include "midunit.h"
#include "settings.h"
#include "usunitimpl.h"
#include "visitors.h"
#include <fmt/format.h>

namespace hooks {

void __fastcall drainAttackOnHitHooked(game::CBatAttackDrain* thisptr,
                                       int /*%edx*/,
                                       game::IMidgardObjectMap* objectMap,
                                       game::BattleMsgData* battleMsgData,
                                       game::CMidgardID* unitId,
                                       game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    const auto& fn = gameFunctions();

    if (fn.isUnitImmuneToAttack(objectMap, battleMsgData, unitId, thisptr->attack, false)) {
        return;
    }

    int attackDamage{};
    int criticalHitDamage{};
    const int fullDamage = fn.computeDamage(objectMap, battleMsgData, thisptr->attack,
                                            &thisptr->unitId, unitId, true, &attackDamage,
                                            &criticalHitDamage);

    auto targetUnit = static_cast<const CMidUnit*>(
        objectMap->vftable->findScenarioObjectById(objectMap, unitId));
    const auto targetInitialHp = targetUnit->currentHp;

    const auto& visitors = VisitorApi::get();
    visitors.changeUnitHp(unitId, -fullDamage, objectMap, 1);

    const auto targetResultingHp = targetUnit->currentHp;
    auto drainDamage = (targetInitialHp - targetResultingHp) * userSettings().vampiricHeal / 100;

    auto attackVftable = static_cast<const IAttackVftable*>(thisptr->attack->vftable);
    drainDamage += attackVftable->getDrain(thisptr->attack, targetInitialHp - targetResultingHp);

    const auto& battle = BattleMsgDataApi::get();
    battle.checkUnitDeath(objectMap, battleMsgData, unitId);

    auto attackerUnit = static_cast<const CMidUnit*>(
        objectMap->vftable->findScenarioObjectById(objectMap, &thisptr->unitId));
    const auto attackerInitialHp = attackerUnit->currentHp;

    visitors.changeUnitHp(&thisptr->unitId, drainDamage, objectMap, 1);

    const auto attackerResultingHp = attackerUnit->currentHp;

    {
        BattleAttackUnitInfo info{};
        info.unitId = *unitId;
        info.unitImplId = targetUnit->unitImpl->unitId;

        const auto hpDiff = targetInitialHp - targetResultingHp;
        if (hpDiff < criticalHitDamage) {
            info.damage = 0;
            info.criticalDamage = hpDiff;
        } else {
            info.damage = hpDiff - criticalHitDamage;
            info.criticalDamage = criticalHitDamage;
        }

        BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);
    }

    {
        BattleAttackUnitInfo info{};
        info.unitId = thisptr->unitId;
        info.unitImplId = attackerUnit->unitImpl->unitId;
        info.damage = attackerResultingHp - attackerInitialHp;

        BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);
    }

    battle.setUnitHp(battleMsgData, unitId, targetUnit->currentHp);
    battle.setUnitHp(battleMsgData, &thisptr->unitId, attackerUnit->currentHp);
}

} // namespace hooks
