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

#include "transformselfhooks.h"
#include "attack.h"
#include "batattacktransformself.h"
#include "battleattackinfo.h"
#include "battlemsgdata.h"
#include "game.h"
#include "globaldata.h"
#include "log.h"
#include "midgardobjectmap.h"
#include "midunit.h"
#include "scripts.h"
#include "unitgenerator.h"
#include "unitimplview.h"
#include "unitview.h"
#include "ussoldier.h"
#include "usunitimpl.h"
#include "utils.h"
#include "visitors.h"
#include <fmt/format.h>

namespace hooks {

static int getTransformSelfLevel(const game::CMidUnit* unit, game::TUsUnitImpl* transformImpl)
{
    const char* filename{"transformSelf.lua"};
    static std::string script{readFile({scriptsFolder() / filename})};
    if (script.empty()) {
        logError("mssProxyError.log", fmt::format("Failed to read '{:s}' script file", filename));
        return 0;
    }

    const auto lua{loadScript(script.c_str())};
    if (!lua) {
        return 0;
    }

    using GetLevel = std::function<int(const bindings::UnitView&, const bindings::UnitImplView&)>;
    auto getLevel = getScriptFunction<GetLevel>(*lua, "getLevel");
    if (!getLevel) {
        return 0;
    }

    try {
        const bindings::UnitView attacker{unit};
        const bindings::UnitImplView impl{transformImpl};

        return (*getLevel)(attacker, impl);
    } catch (const std::exception& e) {
        logError("mssProxyError.log",
                 fmt::format("Failed to run '{:s}' script, reason: '{:s}'", filename, e.what()));
        return 0;
    }
}

void __fastcall transformSelfAttackOnHitHooked(game::CBatAttackTransformSelf* thisptr,
                                               int /*%edx*/,
                                               game::IMidgardObjectMap* objectMap,
                                               game::BattleMsgData* battleMsgData,
                                               game::CMidgardID* targetUnitId,
                                               game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    const auto& fn = gameFunctions();

    CMidgardID targetStackId{emptyId};
    fn.getAllyOrEnemyStackId(&targetStackId, battleMsgData, targetUnitId, true);

    CMidgardID attackerStackId{emptyId};
    fn.getAllyOrEnemyStackId(&attackerStackId, battleMsgData, &thisptr->unitId, true);

    if (*targetUnitId != thisptr->unitId && targetStackId != attackerStackId) {
        auto altAttack = thisptr->altAttack;
        altAttack->vftable->onHit(altAttack, objectMap, battleMsgData, targetUnitId, attackInfo);
        return;
    }

    auto attack = fn.getAttackById(objectMap, &thisptr->id2, thisptr->attackNumber, false);
    auto attackId = IAttackApi::get().getId(attack);

    const auto position = fn.getUnitPositionInGroup(objectMap, &targetStackId, targetUnitId);

    const CMidUnit* targetUnit = fn.findUnitById(objectMap, targetUnitId);
    const CMidgardID targetUnitImplId{targetUnit->unitImpl->unitId};

    auto soldier = gameFunctions().castUnitImplToSoldier(targetUnit->unitImpl);
    auto vftable = static_cast<const IUsSoldierVftable*>(soldier->vftable);
    const auto small = vftable->getSizeSmall(soldier);

    CMidgardID transformImplId{emptyId};
    fn.getSummonUnitImplIdByAttack(&transformImplId, attackId, position, small);

    if (transformImplId == emptyId) {
        return;
    }

    const auto& global = GlobalDataApi::get();
    auto globalData = *global.getGlobalData();

    auto transformImpl = static_cast<TUsUnitImpl*>(
        global.findById(globalData->units, &transformImplId));

    const auto transformLevel = getTransformSelfLevel(targetUnit, transformImpl);

    CUnitGenerator* unitGenerator = globalData->unitGenerator;

    CMidgardID leveledImplId{transformImplId};
    unitGenerator->vftable->generateUnitImplId(unitGenerator, &leveledImplId, &transformImplId,
                                               transformLevel);

    unitGenerator->vftable->generateUnitImpl(unitGenerator, &leveledImplId);

    const auto& visitors = VisitorApi::get();
    visitors.transformUnit(targetUnitId, &leveledImplId, false, objectMap, 1);

    const auto& battle = BattleMsgDataApi::get();

    BattleAttackUnitInfo info{};
    info.unitId = *targetUnitId;
    info.unitImplId = targetUnitImplId;
    BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);

    battle.removeTransformStatuses(targetUnitId, battleMsgData);
    battle.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::TransformSelf, true);

    battle.setUnitHp(battleMsgData, targetUnitId, targetUnit->currentHp);
}

} // namespace hooks
