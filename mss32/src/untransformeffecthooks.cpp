/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "batattackuntransformeffect.h"
#include "battleattackinfo.h"
#include "battlemsgdata.h"
#include "game.h"
#include "midunit.h"
#include "unitutils.h"
#include "ussoldier.h"
#include "usunit.h"
#include "visitors.h"

namespace hooks {

void __fastcall untransformEffectAttackOnHitHooked(game::CBatAttackUntransformEffect* thisptr,
                                                   int /*%edx*/,
                                                   game::IMidgardObjectMap* objectMap,
                                                   game::BattleMsgData* battleMsgData,
                                                   game::CMidgardID* targetUnitId,
                                                   game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    const auto& fn = gameFunctions();

    const CMidUnit* targetUnit = fn.findUnitById(objectMap, targetUnitId);
    const CMidgardID targetUnitImplId{targetUnit->unitImpl->unitId};

    const auto targetSoldier = fn.castUnitImplToSoldier(targetUnit->unitImpl);
    bool prevAttackTwice = targetSoldier && targetSoldier->vftable->getAttackTwice(targetSoldier);

    const auto& visitors = VisitorApi::get();
    visitors.undoTransformUnit(targetUnitId, objectMap, 1);

    updateAttackCountAfterTransformation(battleMsgData, targetUnit, prevAttackTwice);

    BattleAttackUnitInfo info{};
    info.unitId = *targetUnitId;
    info.unitImplId = targetUnitImplId;
    BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);

    const auto& battle = BattleMsgDataApi::get();
    battle.removeTransformStatuses(targetUnitId, battleMsgData);

    battle.setUnitHp(battleMsgData, targetUnitId, targetUnit->currentHp);
}

} // namespace hooks
