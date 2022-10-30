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
#include "customattacks.h"
#include "game.h"
#include "gameutils.h"
#include "globaldata.h"
#include "intset.h"
#include "log.h"
#include "midgardobjectmap.h"
#include "midplayer.h"
#include "midunit.h"
#include "scripts.h"
#include "settings.h"
#include "unitgenerator.h"
#include "unitimplview.h"
#include "unitutils.h"
#include "unitview.h"
#include "ussoldier.h"
#include "usunitimpl.h"
#include "utils.h"
#include "visitors.h"
#include <fmt/format.h>

namespace hooks {

static int getTransformSelfLevel(const game::CMidUnit* unit, game::TUsUnitImpl* transformImpl)
{
    std::optional<sol::environment> env;
    const auto path{scriptsFolder() / "transformSelf.lua"};
    auto getLevel = getScriptFunction(path, "getLevel", env, true, true);
    if (!getLevel) {
        return 0;
    }

    try {
        const bindings::UnitView attacker{unit};
        const bindings::UnitImplView impl{transformImpl};

        return (*getLevel)(attacker, impl);
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                        "Reason: '{:s}'",
                                        path.string(), e.what()));
        return 0;
    }
}

static int getTransformSelfFreeAttackNumberDefault(int attacksDone,
                                                   int attacksRemain,
                                                   bool hadDoubleAttack,
                                                   bool hasDoubleAttack)
{
    if (!hadDoubleAttack && hasDoubleAttack && attacksDone == 0) {
        // Give 2 extra attacks if transforming from single to double attack.
        // attacksDone prevents infinite abuse of 2 extra attacks:
        // do normal attack -> transform to single-attack -> transform back to double-attack
        // -> do normal attack -> ...
        return 2;
    } else if (hadDoubleAttack && !hasDoubleAttack && attacksRemain > 0) {
        // Give nothing if transforming from double to single attack with attacks remain
        return 0;
    } else {
        // Give 1 extra attack to compensate transformation
        return 1;
    }
}

static int getTransformSelfFreeAttackNumber(int attacksDone,
                                            int attacksRemain,
                                            bool hadDoubleAttack,
                                            bool hasDoubleAttack)
{
    std::optional<sol::environment> env;
    auto f = getScriptFunction(scriptsFolder() / "transformSelf.lua", "getFreeAttackNumber", env,
                               false, true);
    if (!f)
        return getTransformSelfFreeAttackNumberDefault(attacksDone, attacksRemain, hadDoubleAttack,
                                                       hasDoubleAttack);

    try {
        return (*f)(attacksDone, attacksRemain, hadDoubleAttack, hasDoubleAttack);
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to run 'getFreeAttackNumber' script.\n"
                                        "Reason: '{:s}'",
                                        e.what()));
        return 0;
    }
}

void giveFreeTransformSelfAttack(game::IMidgardObjectMap* objectMap,
                                 game::BattleMsgData* battleMsgData,
                                 const game::CMidUnit* unit,
                                 bool prevAttackTwice)
{
    using namespace game;

    const auto& battleApi = BattleMsgDataApi::get();

    auto& freeTransformSelf = getCustomAttacks().freeTransformSelf;
    if (freeTransformSelf.turnCount > 0) // Can be 0 if this is the very first turn in battle
        freeTransformSelf.turnCount--;   // Not counting transform action as a turn

    if (freeTransformSelf.used) {
        if (!userSettings().freeTransformSelfAttackInfinite)
            return;

        // Prevents AI from falling into infinite transforming in case of targeting malfunction
        if (battleApi.isAutoBattle(battleMsgData)) {
            return;
        }
        auto player = getPlayer(objectMap, battleMsgData, &unit->id);
        if (player && !player->isHuman)
            return;
    }
    freeTransformSelf.used = true;

    for (auto& turn : battleMsgData->turnsOrder) {
        if (turn.unitId == unit->id) {
            const auto soldier = gameFunctions().castUnitImplToSoldier(unit->unitImpl);
            bool attackTwice = soldier && soldier->vftable->getAttackTwice(soldier);
            turn.attackCount += getTransformSelfFreeAttackNumber(freeTransformSelf.turnCount,
                                                                 turn.attackCount - 1,
                                                                 prevAttackTwice, attackTwice);
            break;
        }
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

    bool targetSelf = thisptr->unitId == *targetUnitId;
    if (!targetSelf) {
        auto altAttack = thisptr->altAttack;
        if (altAttack) {
            altAttack->vftable->onHit(altAttack, objectMap, battleMsgData, targetUnitId,
                                      attackInfo);
            return;
        }
    }

    auto attack = fn.getAttackById(objectMap, &thisptr->id2, thisptr->attackNumber, false);

    CMidgardID targetGroupId{emptyId};
    fn.getAllyOrEnemyGroupId(&targetGroupId, battleMsgData, targetUnitId, true);

    const auto position = fn.getUnitPositionInGroup(objectMap, &targetGroupId, targetUnitId);

    const CMidUnit* targetUnit = fn.findUnitById(objectMap, targetUnitId);
    const CMidgardID targetUnitImplId{targetUnit->unitImpl->id};

    CMidgardID transformImplId{emptyId};
    fn.getSummonUnitImplIdByAttack(&transformImplId, &attack->id, position,
                                   isUnitSmall(targetUnit));

    if (transformImplId == emptyId) {
        return;
    }

    if (userSettings().leveledTransformSelfAttack) {
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

        transformImplId = leveledImplId;
    }

    const auto targetSoldier = fn.castUnitImplToSoldier(targetUnit->unitImpl);
    bool prevAttackTwice = targetSoldier && targetSoldier->vftable->getAttackTwice(targetSoldier);

    const auto& visitors = VisitorApi::get();
    visitors.transformUnit(targetUnitId, &transformImplId, false, objectMap, 1);

    if (!targetSelf)
        updateAttackCountAfterTransformation(battleMsgData, targetUnit, prevAttackTwice);
    else if (userSettings().freeTransformSelfAttack)
        giveFreeTransformSelfAttack(objectMap, battleMsgData, targetUnit, prevAttackTwice);

    BattleAttackUnitInfo info{};
    info.unitId = *targetUnitId;
    info.unitImplId = targetUnitImplId;
    BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);

    const auto& battle = BattleMsgDataApi::get();
    battle.removeTransformStatuses(targetUnitId, battleMsgData);
    battle.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::TransformSelf, true);

    battle.setUnitHp(battleMsgData, targetUnitId, targetUnit->currentHp);
}

void __fastcall transformSelfAttackFillTargetsListHooked(game::CBatAttackTransformSelf* thisptr,
                                                         int /*%edx*/,
                                                         game::IMidgardObjectMap* objectMap,
                                                         game::BattleMsgData* battleMsgData,
                                                         game::TargetSet* targetsList)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& intSetApi = IntSetApi::get();

    CMidgardID unitGroupId{emptyId};
    fn.getAllyOrEnemyGroupId(&unitGroupId, battleMsgData, &thisptr->unitId, true);

    int unitPosition = fn.getUnitPositionInGroup(objectMap, &unitGroupId, &thisptr->unitId);

    auto altAttack = thisptr->altAttack;
    if (altAttack) {
        altAttack->vftable->fillTargetsList(altAttack, objectMap, battleMsgData, targetsList);

        CMidgardID targetGroupId{emptyId};
        altAttack->vftable->getTargetGroupId(altAttack, &targetGroupId, battleMsgData);

        if (targetGroupId != unitGroupId)
            unitPosition = -(unitPosition + 1);
    }

    Pair<TargetSetIterator, bool> tmp{};
    intSetApi.insert(targetsList, &tmp, &unitPosition);
}

} // namespace hooks
