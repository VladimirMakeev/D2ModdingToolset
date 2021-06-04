/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#include "customattackutils.h"
#include "batattack.h"
#include "battlemsgdata.h"
#include "customattacks.h"
#include "dbffile.h"
#include "dynamiccast.h"
#include "game.h"
#include "log.h"
#include "midgardobjectmap.h"
#include "midplayer.h"
#include "midunit.h"
#include "midunitgroup.h"
#include "scripts.h"
#include "unitslotview.h"
#include "ussoldier.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

void fillCustomAttackSources(const std::filesystem::path& dbfFilePath)
{
    using namespace game;

    utils::DbfFile dbf;
    if (!dbf.open(dbfFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbfFilePath.filename().string()));
        return;
    }

    static const std::uint32_t lastBaseSourceWardFlagPosition = 7;
    static const std::array<const char*, 8> baseSources = {
        {"L_WEAPON", "L_MIND", "L_LIFE", "L_DEATH", "L_FIRE", "L_WATER", "L_AIR", "L_EARTH"}};

    auto& customSources = getCustomAttacks().sources;
    std::uint32_t wardFlagPosition = lastBaseSourceWardFlagPosition;
    const auto recordsTotal{dbf.recordsTotal()};
    for (std::uint32_t i = 0; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!dbf.record(record, i)) {
            logError("mssProxyError.log", fmt::format("Could not read record {:d} from {:s}", i,
                                                      dbfFilePath.filename().string()));
            return;
        }

        if (record.isDeleted()) {
            continue;
        }

        std::string text;
        record.value(text, "TEXT");
        text = trimSpaces(text);

        if (std::none_of(std::begin(baseSources), std::end(baseSources),
                         [&text](const char* baseText) { return text == baseText; })) {
            std::string nameId;
            record.value(nameId, "NAME_TXT");

            int immunityPower = 5; // 5 is the default
            record.value(immunityPower, "IMMUNE_POW");

            logDebug(
                "customAttacks.log",
                fmt::format("Found custom attack source {:s}, name id {:s}, immunity power {:d}",
                            text, nameId, immunityPower));

            customSources.push_back(
                {LAttackSource{AttackSourceCategories::vftable(), nullptr, (AttackSourceId)-1},
                 text, nameId, (double)immunityPower, ++wardFlagPosition});
        }
    }
}

void fillCustomAttackReaches(const std::filesystem::path& dbfFilePath)
{
    using namespace game;

    utils::DbfFile dbf;
    if (!dbf.open(dbfFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbfFilePath.filename().string()));
        return;
    }

    static const std::array<const char*, 3> baseReaches = {{"L_ALL", "L_ANY", "L_ADJACENT"}};

    auto& customReaches = getCustomAttacks().reaches;
    const auto recordsTotal{dbf.recordsTotal()};
    for (std::uint32_t i = 0; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!dbf.record(record, i)) {
            logError("mssProxyError.log", fmt::format("Could not read record {:d} from {:s}", i,
                                                      dbfFilePath.filename().string()));
            return;
        }

        if (record.isDeleted()) {
            continue;
        }

        std::string text;
        record.value(text, "TEXT");
        text = trimSpaces(text);

        if (std::none_of(std::begin(baseReaches), std::end(baseReaches),
                         [&text](const char* baseText) { return text == baseText; })) {
            std::string reachTxt;
            record.value(reachTxt, "REACH_TXT");

            std::string targetsTxt;
            record.value(targetsTxt, "TARGET_TXT");

            std::string selectionScript;
            record.value(selectionScript, "SEL_SCRIPT");

            std::string attackScript;
            record.value(attackScript, "ATT_SCRIPT");

            logDebug("customAttacks.log", fmt::format("Found custom attack reach {:s}", text));

            customReaches.push_back(
                {LAttackReach{AttackReachCategories::vftable(), nullptr, (AttackReachId)-1}, text,
                 reachTxt, targetsTxt, trimSpaces(selectionScript), trimSpaces(attackScript)});
        }
    }
}

void filterTargetsToSelect(const std::string scriptFile,
                           const bindings::UnitSlotView& attacker,
                           const std::vector<bindings::UnitSlotView>& allies,
                           std::vector<bindings::UnitSlotView>& targets,
                           bool targetsAreAllies)
{
    const auto path{scriptsFolder() / scriptFile};
    const auto lua{loadScriptFile(path, true, true)};
    if (!lua) {
        return;
    }

    using FilterTargetsToSelect = std::function<void(const bindings::UnitSlotView&,
                                                     const std::vector<bindings::UnitSlotView>&,
                                                     std::vector<bindings::UnitSlotView>&, bool)>;
    auto filterTargetsToSelect = getScriptFunction<FilterTargetsToSelect>(*lua,
                                                                          "filterTargetsToSelect");
    if (!filterTargetsToSelect) {
        showErrorMessageBox(
            fmt::format("Could not find function 'filterTargetsToSelect' in script '{:s}'.\n"
                        "Make sure function exists and has correct signature.",
                        path.string()));
        return;
    }

    try {
        (*filterTargetsToSelect)(attacker, allies, targets, targetsAreAllies);
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                        "Reason: '{:s}'",
                                        path.string(), e.what()));
    }
}

std::vector<bindings::UnitSlotView> getTargetSlots(const game::IMidgardObjectMap* objectMap,
                                                   const game::BattleMsgData* battleMsgData,
                                                   const game::IBatAttack* batAttack,
                                                   const game::CMidgardID* targetGroupId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battle = BattleMsgDataApi::get();

    void* tmp{};
    auto targetGroup = fn.getStackFortRuinGroup(tmp, objectMap, targetGroupId);

    std::vector<bindings::UnitSlotView> value;
    bool isSummonAttack = batAttack->vftable->method17(batAttack, battleMsgData);
    for (size_t i = 0; i < std::size(targetGroup->positions); ++i) {
        CMidgardID targetUnitId = targetGroup->positions[i];
        if (isSummonAttack) {
            if (targetUnitId == emptyId) {
                value.emplace_back(bindings::UnitSlotView(nullptr, i, targetGroupId));
            } else if (battle.canPerformAttackOnUnitWithStatusCheck(objectMap, battleMsgData,
                                                                    batAttack, &targetUnitId)) {
                auto targetUnit = fn.findUnitById(objectMap, &targetUnitId);
                value.emplace_back(bindings::UnitSlotView(targetUnit, i, targetGroupId));
            }
        } else {
            if (targetUnitId == emptyId)
                continue;

            auto targetUnit = fn.findUnitById(objectMap, &targetUnitId);
            if (i % 2) {
                const IUsSoldier* targetSoldier = fn.castUnitImplToSoldier(targetUnit->unitImpl);
                if (!targetSoldier->vftable->getSizeSmall(targetSoldier))
                    continue;
            }

            if (battle.canPerformAttackOnUnitWithStatusCheck(objectMap, battleMsgData, batAttack,
                                                             &targetUnitId)) {
                value.emplace_back(bindings::UnitSlotView(targetUnit, i, targetGroupId));
            }
        }
    }

    return value;
}

std::vector<bindings::UnitSlotView> getAllySlots(const game::IMidgardObjectMap* objectMap,
                                                 const game::BattleMsgData* battleMsgData,
                                                 const game::IBatAttack* batAttack,
                                                 const game::CMidgardID* unitGroupId,
                                                 const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battle = BattleMsgDataApi::get();

    void* tmp{};
    auto unitGroup = fn.getStackFortRuinGroup(tmp, objectMap, unitGroupId);

    std::vector<bindings::UnitSlotView> value;
    for (size_t i = 0; i < std::size(unitGroup->positions); ++i) {
        CMidgardID allyUnitId = unitGroup->positions[i];
        if (allyUnitId == emptyId || allyUnitId == *unitId)
            continue;

        auto allyUnit = fn.findUnitById(objectMap, &allyUnitId);
        if (i % 2) {
            const IUsSoldier* allySoldier = fn.castUnitImplToSoldier(allyUnit->unitImpl);
            if (!allySoldier->vftable->getSizeSmall(allySoldier))
                continue;
        }

        if (battle.getUnitStatus(battleMsgData, &allyUnitId, BattleStatus::Dead)
            || battle.getUnitStatus(battleMsgData, &allyUnitId, BattleStatus::Retreated)
            || battle.getUnitStatus(battleMsgData, &allyUnitId, BattleStatus::Hidden)) {
            continue;
        }

        value.emplace_back(bindings::UnitSlotView(allyUnit, i, unitGroupId));
    }

    return value;
}

void fillTargetsListForCustomAttackReach(const game::IMidgardObjectMap* objectMap,
                                         const game::BattleMsgData* battleMsgData,
                                         const game::IBatAttack* batAttack,
                                         const game::CMidgardID* targetGroupId,
                                         const game::CMidgardID* unitGroupId,
                                         const game::CMidgardID* unitId,
                                         const CustomAttackReach& attackReach,
                                         game::TargetsList* value)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& listApi = TargetsListApi::get();
    const auto& groupApi = CMidUnitGroupApi::get();
    const auto& battle = BattleMsgDataApi::get();

    listApi.clear(value);

    void* tmp{};
    auto unitGroup = fn.getStackFortRuinGroup(tmp, objectMap, unitGroupId);

    auto unit = fn.findUnitById(objectMap, unitId);
    int unitPosition = groupApi.getUnitPosition(unitGroup, unitId);
    bindings::UnitSlotView attacker(unit, unitPosition, unitGroupId);

    auto targets = getTargetSlots(objectMap, battleMsgData, batAttack, targetGroupId);
    auto allies = getAllySlots(objectMap, battleMsgData, batAttack, unitGroupId, unitId);
    filterTargetsToSelect(attackReach.selectionScript, attacker, allies, targets,
                          unitGroupId == targetGroupId);

    for (const auto& target : targets) {
        Pair<TargetsListIterator, bool> tmp{};
        int position = target.getPosition();
        listApi.insert(value, &tmp, &position);
    }
}

bool shouldExcludeImmuneTargets(const game::IMidgardObjectMap* objectMap,
                                const game::BattleMsgData* battleMsgData,
                                const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& id = CMidgardIDApi::get();
    const auto& rtti = RttiApi::rtti();
    const auto& battle = BattleMsgDataApi::get();

    if (battle.isAutoBattle(battleMsgData))
        return true;

    CMidgardID playerId{};
    id.validateId(&playerId, battle.isUnitAttacker(battleMsgData, unitId)
                                 ? battleMsgData->attackerPlayerId
                                 : battleMsgData->defenderPlayerId);
    if (playerId == emptyId)
        return true;

    auto playerObj = objectMap->vftable->findScenarioObjectById(objectMap, &playerId);
    const auto dynamicCast = RttiApi::get().dynamicCast;
    CMidPlayer* player = (CMidPlayer*)dynamicCast(playerObj, 0, rtti.IMidScenarioObjectType,
                                                  rtti.CMidPlayerType, 0);

    return !player || !player->isHuman;
}

void excludeImmuneTargets(const game::IMidgardObjectMap* objectMap,
                          const game::BattleMsgData* battleMsgData,
                          const game::IAttack* attack,
                          const game::CMidgardID* unitGroupId,
                          const game::CMidgardID* targetGroupId,
                          game::TargetsList* value)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& listApi = TargetsListApi::get();
    const auto& groupApi = CMidUnitGroupApi::get();

    void* tmp{};
    auto unitGroup = fn.getStackFortRuinGroup(tmp, objectMap, unitGroupId);
    auto targetGroup = fn.getStackFortRuinGroup(tmp, objectMap, targetGroupId);

    TargetsListIterator it, end;
    listApi.end(value, &end);
    for (listApi.begin(value, &it); !listApi.equals(&it, &end); listApi.preinc(&it)) {
        int targetPosition = *listApi.dereference(&it);

        CMidgardID unitId{};
        if (targetPosition >= 0) {
            unitId = *groupApi.getUnitIdByPosition(targetGroup, targetPosition);
        } else {
            unitId = *groupApi.getUnitIdByPosition(unitGroup, -(targetPosition + 1));
        }

        if (fn.isUnitImmuneToAttack(objectMap, battleMsgData, &unitId, attack, true)) {
            listApi.erase(value, &targetPosition);
        }
    }
}

} // namespace hooks
