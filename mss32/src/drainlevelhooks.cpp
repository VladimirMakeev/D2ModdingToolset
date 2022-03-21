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

#include "drainlevelhooks.h"
#include "attack.h"
#include "batattackdrainlevel.h"
#include "batattackutils.h"
#include "battleattackinfo.h"
#include "battlemsgdata.h"
#include "game.h"
#include "gameutils.h"
#include "globaldata.h"
#include "itemview.h"
#include "log.h"
#include "midgardobjectmap.h"
#include "midunit.h"
#include "scripts.h"
#include "settings.h"
#include "unitgenerator.h"
#include "unitimplview.h"
#include "unitutils.h"
#include "unitview.h"
#include "usleader.h"
#include "usracialsoldier.h"
#include "ussoldier.h"
#include "usunitimpl.h"
#include "utils.h"
#include "visitors.h"

namespace hooks {

static int getDrainLevel(const game::CMidUnit* unit,
                         const game::CMidUnit* targetUnit,
                         const game::IMidgardObjectMap* objectMap,
                         const game::CMidgardID* unitOrItemId)
{
    using namespace game;

    const auto path{scriptsFolder() / "drainLevel.lua"};
    const auto lua{loadScriptFile(path, true, true)};
    if (!lua) {
        return 0;
    }

    using GetLevel = std::function<int(const bindings::UnitView&, const bindings::UnitView&,
                                       const bindings::ItemView*)>;
    auto getLevel = getScriptFunction<GetLevel>(*lua, "getLevel");
    if (!getLevel) {
        showErrorMessageBox(fmt::format("Could not find function 'getLevel' in script '{:s}'.\n"
                                        "Make sure function exists and has correct signature.",
                                        path.string()));
        return 0;
    }

    try {
        const bindings::UnitView attacker{unit};
        const bindings::UnitView target{targetUnit};

        if (CMidgardIDApi::get().getType(unitOrItemId) == IdType::Item) {
            const bindings::ItemView itemView{unitOrItemId, objectMap};
            return (*getLevel)(attacker, target, &itemView);
        } else
            return (*getLevel)(attacker, target, nullptr);
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                        "Reason: '{:s}'",
                                        path.string(), e.what()));
        return 0;
    }
}

void __fastcall drainLevelAttackOnHitHooked(game::CBatAttackDrainLevel* thisptr,
                                            int /*%edx*/,
                                            game::IMidgardObjectMap* objectMap,
                                            game::BattleMsgData* battleMsgData,
                                            game::CMidgardID* targetUnitId,
                                            game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    const auto& fn = gameFunctions();

    if (fn.isUnitImmuneToAttack(objectMap, battleMsgData, targetUnitId, thisptr->attack, false))
        return;

    const CMidUnit* targetUnit = fn.findUnitById(objectMap, targetUnitId);
    const CMidgardID targetUnitImplId{targetUnit->unitImpl->unitId};

    int targetLevel = 1;
    auto targetSoldier = fn.castUnitImplToSoldier(targetUnit->unitImpl);
    if (targetSoldier)
        targetLevel = targetSoldier->vftable->getLevel(targetSoldier);

    int drainLevel = targetLevel - 1;
    if (userSettings().leveledDrainLevelAttack) {
        const CMidUnit* unit = fn.findUnitById(objectMap, &thisptr->unitId);
        drainLevel = getDrainLevel(unit, targetUnit, objectMap, &thisptr->unitOrItemId);
    }

    const auto& global = GlobalDataApi::get();
    auto globalData = *global.getGlobalData();

    CMidgardID transformImplId = emptyId;
    for (int i = targetLevel; i > drainLevel; i--) {
        auto unitImpl = targetUnit->unitImpl;
        if (transformImplId != emptyId)
            unitImpl = static_cast<TUsUnitImpl*>(
                global.findById(globalData->units, &transformImplId));

        auto racialSoldier = fn.castUnitImplToRacialSoldier(unitImpl);
        if (racialSoldier)
            transformImplId = *racialSoldier->vftable->getPrevUnitImplId(racialSoldier);
        else {
            auto leader = fn.castUnitImplToLeader(unitImpl);
            if (leader)
                transformImplId = *leader->vftable->getPrevUnitImplId(leader);
        }

        if (transformImplId == emptyId)
            return;

        if (CMidgardIDApi::get().getType(&transformImplId) == IdType::UnitGenerated) {
            CUnitGenerator* unitGenerator = globalData->unitGenerator;
            unitGenerator->vftable->generateUnitImpl(unitGenerator, &transformImplId);
        }
    }

    bool prevAttackTwice = targetSoldier && targetSoldier->vftable->getAttackTwice(targetSoldier);

    const auto& visitors = VisitorApi::get();
    visitors.transformUnit(targetUnitId, &transformImplId, false, objectMap, 1);

    updateAttackCountAfterTransformation(battleMsgData, targetUnit, prevAttackTwice);

    BattleAttackUnitInfo info{};
    info.unitId = *targetUnitId;
    info.unitImplId = targetUnitImplId;
    BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);

    const auto& battle = BattleMsgDataApi::get();
    battle.removeTransformStatuses(targetUnitId, battleMsgData);
    battle.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::TransformDrainLevel, true);

    battle.setUnitHp(battleMsgData, targetUnitId, targetUnit->currentHp);
}

} // namespace hooks
