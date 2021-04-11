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
#include "attack.h"
#include "batattackdoppelganger.h"
#include "battleattackinfo.h"
#include "battlemsgdata.h"
#include "game.h"
#include "globaldata.h"
#include "immunecat.h"
#include "log.h"
#include "midgardobjectmap.h"
#include "midunit.h"
#include "scripts.h"
#include "settings.h"
#include "unitgenerator.h"
#include "unitview.h"
#include "ussoldier.h"
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

bool isAllyTarget(game::CBatAttackDoppelganger* thisptr,
                  game::BattleMsgData* battleMsgData,
                  game::CMidgardID* targetUnitId)
{
    using namespace game;

    CMidgardID unitStackId{};
    gameFunctions().getAllyOrEnemyStackId(&unitStackId, battleMsgData, &thisptr->unitId, true);

    CMidgardID targetUnitStackId{};
    gameFunctions().getAllyOrEnemyStackId(&targetUnitStackId, battleMsgData, targetUnitId, true);

    return unitStackId == targetUnitStackId;
}

bool __fastcall doppelgangerAttackCanPerformHooked(game::CBatAttackDoppelganger* thisptr,
                                                   int /*%edx*/,
                                                   game::IMidgardObjectMap* objectMap,
                                                   game::BattleMsgData* battleMsgData,
                                                   game::CMidgardID* targetUnitId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    if (battle.isUnitTransformed(&thisptr->unitId, battleMsgData) || !thisptr->canTransform) {
        return thisptr->altAttack->vftable->canPerform(thisptr->altAttack, objectMap, battleMsgData,
                                                       targetUnitId);
    }

    if (battle.getUnitStatus(battleMsgData, targetUnitId, BattleStatus::Dead))
        return false;

    const auto& fn = gameFunctions();
    const CMidUnit* targetUnit = fn.findUnitById(objectMap, targetUnitId);

    const IUsUnit* targetUnitImpl = targetUnit->unitImpl;
    const LUnitCategory* unitCategory = targetUnitImpl->vftable->getCategory(targetUnitImpl);
    if (unitCategory->id == UnitId::Illusion || unitCategory->id == UnitId::Guardian)
        return false;

    const IUsSoldier* targetSoldier = fn.castUnitImplToSoldier(targetUnit->unitImpl);
    if (!targetSoldier->vftable->getSizeSmall(targetSoldier))
        return false;

    const IAttack* targetAttack = targetSoldier->vftable->getAttackById(targetSoldier);
    const LAttackClass* targetAttackClass = targetAttack->vftable->getAttackClass(targetAttack);
    if (targetAttackClass->id == AttackClassId::Doppelganger)
        return false;

    bool ally = isAllyTarget(thisptr, battleMsgData, targetUnitId);
    if (ally && !userSettings().doppelgangerRespectsAllyImmunity)
        return true;
    if (!ally && !userSettings().doppelgangerRespectsEnemyImmunity)
        return true;

    IAttack* attack = fn.getAttackById(objectMap, &thisptr->attackImplUnitId, thisptr->attackNumber,
                                       false);
    return !fn.isUnitImmuneToAttack(objectMap, battleMsgData, targetUnitId, attack, true);
}

/*
 * CBatLogic::battleTurn incorrectly handles immunities to doppelganger and transform self
 * attacks. Had to call removeUnitAttackSourceWard and removeUnitAttackClassWard here as a
 * backdoor. CBatLogic::battleTurn needs to be rewritten someday.
 */
bool __fastcall doppelgangerAttackIsImmuneHooked(game::CBatAttackDoppelganger* thisptr,
                                                 int /*%edx*/,
                                                 game::IMidgardObjectMap* objectMap,
                                                 game::BattleMsgData* battleMsgData,
                                                 game::CMidgardID* unitId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    if (battle.isUnitTransformed(&thisptr->unitId, battleMsgData) || !thisptr->canTransform) {
        return thisptr->altAttack->vftable->isImmune(thisptr->altAttack, objectMap, battleMsgData,
                                                     unitId);
    }

    bool ally = isAllyTarget(thisptr, battleMsgData, unitId);
    if (ally && !userSettings().doppelgangerRespectsAllyImmunity)
        return false;
    if (!ally && !userSettings().doppelgangerRespectsEnemyImmunity)
        return false;

    const auto& fn = gameFunctions();
    IAttack* attack = fn.getAttackById(objectMap, &thisptr->attackImplUnitId, thisptr->attackNumber,
                                       false);

    const CMidUnit* targetUnit = fn.findUnitById(objectMap, unitId);
    const IUsSoldier* targetSoldier = fn.castUnitImplToSoldier(targetUnit->unitImpl);

    const LAttackSource* attackSource = attack->vftable->getAttackSource(attack);
    const LImmuneCat* immuneCat = targetSoldier->vftable->getImmuneByAttackSource(targetSoldier,
                                                                                  attackSource);

    const LAttackClass* attackClass = attack->vftable->getAttackClass(attack);
    const LImmuneCat* immuneCatC = targetSoldier->vftable->getImmuneByAttackClass(targetSoldier,
                                                                                  attackClass);

    bool result = false;
    if (immuneCat->id == ImmuneCategories::get().once->id) {
        result = !battle.isUnitAttackSourceWardRemoved(battleMsgData, unitId, attackSource);
        if (result)
            battle.removeUnitAttackSourceWard(battleMsgData, unitId, attackSource);
    } else if (immuneCat->id == ImmuneCategories::get().always->id) {
        result = true;
    }

    if (immuneCatC->id == ImmuneCategories::get().once->id) {
        result = !battle.isUnitAttackClassWardRemoved(battleMsgData, unitId, attackClass);
        if (result)
            battle.removeUnitAttackClassWard(battleMsgData, unitId, attackClass);
    } else if (immuneCatC->id == ImmuneCategories::get().always->id) {
        result = true;
    }

    return result;
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

    if (battle.isUnitTransformed(&thisptr->unitId, battleMsgData) || !thisptr->canTransform) {
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
