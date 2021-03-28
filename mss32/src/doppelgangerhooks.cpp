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
#include "log.h"
#include "midgardobjectmap.h"
#include "midunit.h"
#include "scripts.h"
#include "unitgenerator.h"
#include "unitview.h"
#include "usunitimpl.h"
#include "utils.h"
#include "visitors.h"
#include <fmt/format.h>

namespace hooks {

static int getDoppelgangerTransformLevel(const game::CMidUnit* doppelganger,
                                         const game::CMidUnit* targetUnit)
{
    const char* filename{"doppelganger.lua"};
    static std::string script{readFile({scriptsFolder() / filename})};
    if (script.empty()) {
        const auto message{fmt::format("Failed to read '{:s}' script file", filename)};

        logError("mssProxyError.log", message);
        showErrorMessageBox(message);
        return 0;
    }

    const auto lua{loadScript(script.c_str())};
    if (!lua) {
        showErrorMessageBox(fmt::format("Failed to load '{:s}' script file.\n"
                                        "See 'mssProxyError.log' for details.",
                                        filename));
        return 0;
    }

    using GetLevel = std::function<int(const bindings::UnitView&, const bindings::UnitView&)>;
    auto getLevel = getScriptFunction<GetLevel>(*lua, "getLevel");
    if (!getLevel) {
        showErrorMessageBox(fmt::format("Could not find function 'getLevel' in script '{:s}'.\n"
                                        "Make sure function exists and has correct signature.",
                                        filename));
        return 0;
    }

    try {
        const bindings::UnitView attacker{doppelganger};
        const bindings::UnitView target{targetUnit};

        return (*getLevel)(attacker, target);
    } catch (const std::exception& e) {
        const auto message{fmt::format("Failed to run '{:s}' script.\n"
                                       "Reason: '{:s}'",
                                       filename, e.what())};
        logError("mssProxyError.log", message);
        showErrorMessageBox(message);
        return 0;
    }
}

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
    const auto transformLevel = getDoppelgangerTransformLevel(unit, targetUnit);

    CMidgardID transformUnitImplId{targetUnit->unitImpl->unitId};
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
