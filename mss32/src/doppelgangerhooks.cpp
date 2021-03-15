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

#include "doppelgangerhooks.h"
#include "batattackdoppelganger.h"
#include "battleattackinfo.h"
#include "battlemsgdata.h"
#include "game.h"
#include "globaldata.h"
#include "midgardobjectmap.h"
#include "midunit.h"
#include "unitgenerator.h"
#include "usunitimpl.h"
#include "visitors.h"

namespace hooks {

void __fastcall doppelgangerAttackOnHitHooked(game::CBatAttackDoppelganger* thisptr,
                                              int /*%edx*/,
                                              game::IMidgardObjectMap* objectMap,
                                              game::BattleMsgData* battleMsgData,
                                              game::CMidgardID* targetUnitId,
                                              game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();

    if (battle.isUnitTransformed(&thisptr->unitId, battleMsgData) || !thisptr->unknown) {
        thisptr->altAttack->vftable->onHit(thisptr->altAttack, objectMap, battleMsgData,
                                           targetUnitId, attackInfo);
        return;
    }

    const auto& fn = gameFunctions();

    CMidUnit* targetUnit = fn.findUnitById(objectMap, targetUnitId);
    CMidgardID targetUnitImplId = targetUnit->unitImpl->unitId;

    CMidgardID globalTargetUnitImplId;
    CUnitGenerator* unitGenerator = (*(GlobalDataApi::get().getGlobalData()))->unitGenerator;
    unitGenerator->vftable->getGlobalUnitImplId(unitGenerator, &globalTargetUnitImplId,
                                                &targetUnit->unitImpl->unitId);

    const CMidUnit* unit = fn.findUnitById(objectMap, &thisptr->unitId);
    const int unitLevel = fn.getUnitLevelByImplId(&unit->unitImpl->unitId);
    const int targetLevel = fn.getUnitLevelByImplId(&targetUnitImplId);
    const int globalLevel = fn.getUnitLevelByImplId(&globalTargetUnitImplId);

    int transformLevel = unitLevel < targetLevel ? unitLevel : targetLevel;
    if (transformLevel < globalLevel)
        transformLevel = globalLevel;

    CMidgardID transformUnitImplId;
    unitGenerator->vftable->generateUnitImplId(unitGenerator, &transformUnitImplId,
                                               &targetUnit->unitImpl->unitId, transformLevel);

    unitGenerator->vftable->generateUnitImpl(unitGenerator, &transformUnitImplId);

    const auto& visitors = VisitorApi::get();
    visitors.transformUnit(&thisptr->unitId, &transformUnitImplId, false, objectMap, 1);

    BattleAttackUnitInfo info{};
    info.unitId = thisptr->unitId;
    info.unitImplId = unit->unitImpl->unitId;
    BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);

    battle.removeTransformStatuses(&thisptr->unitId, battleMsgData);

    battle.setUnitStatus(battleMsgData, &thisptr->unitId, BattleStatus::TransformDoppelganger,
                         true);

    battle.setUnitHp(battleMsgData, &thisptr->unitId, unit->currentHp);
}

} // namespace hooks
