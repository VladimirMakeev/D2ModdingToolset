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

#include "summonhooks.h"
#include "attack.h"
#include "batattacksummon.h"
#include "battleattackinfo.h"
#include "battlemsgdata.h"
#include "game.h"
#include "globaldata.h"
#include "itemview.h"
#include "log.h"
#include "midgardobjectmap.h"
#include "midunit.h"
#include "midunitgroup.h"
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

static int getSummonLevel(const game::CMidUnit* summoner,
                          game::TUsUnitImpl* summonImpl,
                          const game::IMidgardObjectMap* objectMap,
                          const game::CMidgardID* unitOrItemId)
{
    using namespace game;

    std::optional<sol::state> lua;
    const auto path{scriptsFolder() / "summon.lua"};
    using GetLevel = std::function<int(const bindings::UnitView&, const bindings::UnitImplView&,
                                       const bindings::ItemView*)>;
    auto getLevel = getScriptFunction<GetLevel>(path, "getLevel", lua, true, true);
    if (!getLevel) {
        return 0;
    }

    try {
        const bindings::UnitView summonerUnit{summoner};
        const bindings::UnitImplView impl{summonImpl};

        if (CMidgardIDApi::get().getType(unitOrItemId) == IdType::Item) {
            const bindings::ItemView itemView{unitOrItemId, objectMap};
            return (*getLevel)(summonerUnit, impl, &itemView);
        } else
            return (*getLevel)(summonerUnit, impl, nullptr);
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                        "Reason: '{:s}'",
                                        path.string(), e.what()));
        return 0;
    }
}

void __fastcall summonAttackOnHitHooked(game::CBatAttackSummon* thisptr,
                                        int /*%edx*/,
                                        game::IMidgardObjectMap* objectMap,
                                        game::BattleMsgData* battleMsgData,
                                        game::CMidgardID* targetUnitId,
                                        game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    CMidgardID targetGroupId{emptyId};
    thisptr->vftable->getTargetGroupId(thisptr, &targetGroupId, battleMsgData);

    const auto& fn = gameFunctions();

    void* tmp{};
    auto targetGroup = fn.getStackFortRuinGroup(tmp, objectMap, &targetGroupId);

    const auto& idApi = CMidgardIDApi::get();
    CMidgardID maybeSummonId{emptyId};
    idApi.isSummonUnitId(&maybeSummonId, targetUnitId);

    const auto& groupApi = CMidUnitGroupApi::get();

    int position{-1};
    if (maybeSummonId == emptyId) {
        // Target unit exists, we can get its position from group directly
        position = groupApi.getUnitPosition(targetGroup, targetUnitId);
    } else {
        // Target unit does not exist, 'targetUnitId' contains special id for summon.
        position = idApi.summonUnitIdToPosition(&maybeSummonId);
    }

    if (position < 0 || position >= 6) {
        logError("mssProxyError.log", fmt::format("Wrong position for summon. Target unit id {:s}",
                                                  idToString(targetUnitId)));
        return;
    }

    const bool positionEven = position % 2 == 0;
    const int bigUnitSecondPosition = positionEven ? position + 1 : position - 1;

    CMidgardID bigUnitSummonId{emptyId};
    idApi.summonUnitIdFromPosition(&bigUnitSummonId, bigUnitSecondPosition);

    const bool canSummonBig = thisptr->vftable->canPerform(thisptr, objectMap, battleMsgData,
                                                           &bigUnitSummonId);

    auto attackId{IAttackApi::get().getId(thisptr->attack)};

    CMidgardID summonImplId{emptyId};
    fn.getSummonUnitImplId(&summonImplId, objectMap, attackId, &targetGroupId, targetUnitId,
                           canSummonBig);
    if (summonImplId == emptyId) {
        logError("mssProxyError.log",
                 fmt::format("Could not find unit impl id for summon. "
                             "Attack {:s}, group {:s}, target unit {:s}, can summon big {:d}",
                             idToString(attackId), idToString(&targetGroupId),
                             idToString(targetUnitId), (int)canSummonBig));
        return;
    }

    const auto& global = GlobalDataApi::get();
    auto globalData = *global.getGlobalData();

    auto summoner = fn.findUnitById(objectMap, &thisptr->unitId);
    auto summonImpl = static_cast<TUsUnitImpl*>(global.findById(globalData->units, &summonImplId));
    const auto summonLevel = getSummonLevel(summoner, summonImpl, objectMap,
                                            &thisptr->unitOrItemId);

    CUnitGenerator* unitGenerator = globalData->unitGenerator;

    CMidgardID leveledImplId{summonImplId};
    unitGenerator->vftable->generateUnitImplId(unitGenerator, &leveledImplId, &summonImplId,
                                               summonLevel);
    unitGenerator->vftable->generateUnitImpl(unitGenerator, &leveledImplId);

    auto unitImpl = static_cast<TUsUnitImpl*>(global.findById(globalData->units, &leveledImplId));
    if (!unitImpl) {
        logError("mssProxyError.log",
                 fmt::format("Could not find unit impl by id {:s}. Base impl id {:s}, level {:d}",
                             idToString(&leveledImplId), idToString(&summonImplId), summonLevel));
        return;
    }

    auto soldier = fn.castUnitImplToSoldier(unitImpl);
    const auto summonIsBig = !soldier->vftable->getSizeSmall(soldier);
    if (summonIsBig && !positionEven) {
        position--;
    }

    const auto& visitors = VisitorApi::get();
    visitors.forceUnitMax(&targetGroupId, 6, objectMap, 1);

    const auto& battle = BattleMsgDataApi::get();

    auto existingUnitId = groupApi.getUnitIdByPosition(targetGroup, position);
    if (*existingUnitId != emptyId) {
        // Remember unit id before extraction.
        // 'existingUnitId' will point to another id after extraction.
        const CMidgardID unitIdToExtract = *existingUnitId;

        if (!visitors.extractUnitFromGroup(&unitIdToExtract, &targetGroupId, objectMap, 1)) {
            logError("mssProxyError.log",
                     fmt::format("Failed to extract unit {:s} from group {:s}, position {:d}",
                                 idToString(&unitIdToExtract), idToString(&targetGroupId),
                                 position));
            return;
        }

        battle.setUnitShown(battleMsgData, &unitIdToExtract, false);
    }

    if (summonIsBig) {
        auto secondExistingUnitId = groupApi.getUnitIdByPosition(targetGroup, position + 1);
        if (*secondExistingUnitId != emptyId) {
            const CMidgardID unitIdToExtract = *secondExistingUnitId;

            if (!visitors.extractUnitFromGroup(&unitIdToExtract, &targetGroupId, objectMap, 1)) {
                logError("mssProxyError.log",
                         fmt::format("Failed to extract unit {:s} from group {:s}, position {:d}",
                                     idToString(&unitIdToExtract), idToString(&targetGroupId),
                                     position + 1));
                return;
            }

            battle.setUnitShown(battleMsgData, &unitIdToExtract, false);
        }
    }

    CMidgardID newUnitId{emptyId};
    objectMap->vftable->createScenarioId(objectMap, &newUnitId, IdType::Unit);

    int creationTurn{1};
    if (!visitors.addUnitToGroup(&leveledImplId, &targetGroupId, position, &creationTurn, 1,
                                 objectMap, 1)) {
        logError("mssProxyError.log",
                 fmt::format("Could not add unit impl {:s} to group {:s} at position {:d}",
                             idToString(&leveledImplId), idToString(&targetGroupId), position));
        return;
    }

    const CMidUnit* newUnit = fn.findUnitById(objectMap, &newUnitId);
    if (!newUnit) {
        logError("mssProxyError.log",
                 fmt::format("Could not find unit with id {:s}", idToString(&newUnitId)));
        return;
    }

    BattleAttackUnitInfo info{};
    info.unitId = newUnitId;
    info.unitImplId = newUnit->unitImpl->unitId;
    BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);

    battle.addSummonedUnit(battleMsgData, objectMap, &newUnitId, &targetGroupId);
    battle.setUnitStatus(battleMsgData, &newUnitId, BattleStatus::Summon, true);
    battle.setSummonOwner(battleMsgData, &newUnitId, &thisptr->unitId);

    visitors.forceUnitMax(&targetGroupId, -1, objectMap, 1);
}

} // namespace hooks
