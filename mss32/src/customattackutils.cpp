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
#include "attack.h"
#include "attackclasscat.h"
#include "attackimpl.h"
#include "attackutils.h"
#include "batattack.h"
#include "batattacktransformself.h"
#include "battlemsgdata.h"
#include "customattacks.h"
#include "dbffile.h"
#include "dynamiccast.h"
#include "game.h"
#include "gameutils.h"
#include "idlistutils.h"
#include "log.h"
#include "midgardobjectmap.h"
#include "midplayer.h"
#include "midunit.h"
#include "midunitgroup.h"
#include "scripts.h"
#include "settings.h"
#include "targetslistutils.h"
#include "unitslotview.h"
#include "unitutils.h"
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

            int immunityAiRating = 5; // 5 is the default
            record.value(immunityAiRating, "IMMU_AI_R");

            logDebug("customAttacks.log",
                     fmt::format(
                         "Found custom attack source {:s}, name id {:s}, immunity ai rating {:d}",
                         text, nameId, immunityAiRating));

            customSources.push_back(
                {LAttackSource{AttackSourceCategories::vftable(), nullptr, (AttackSourceId)-1},
                 text, nameId, (double)immunityAiRating, ++wardFlagPosition});
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

            bool markAttackTargets = false;
            record.value(markAttackTargets, "MRK_TARGTS");

            bool melee = false;
            record.value(melee, "MELEE");

            int maxTargets = 1; // 1 is the default
            record.value(maxTargets, "MAX_TARGTS");

            logDebug("customAttacks.log", fmt::format("Found custom attack reach {:s}", text));

            customReaches.push_back(
                {LAttackReach{AttackReachCategories::vftable(), nullptr, (AttackReachId)-1}, text,
                 reachTxt, targetsTxt, trimSpaces(selectionScript), trimSpaces(attackScript),
                 markAttackTargets, melee, (std::uint32_t)maxTargets});
        }
    }
}

UnitSlots getTargetsToSelectOrAttack(const std::string& scriptFile,
                                     const bindings::UnitSlotView& attacker,
                                     const bindings::UnitSlotView& selected,
                                     const UnitSlots& allies,
                                     const UnitSlots& targets,
                                     bool targetsAreAllies)
{
    const auto path{scriptsFolder() / scriptFile};
    const auto lua{loadScriptFile(path, true, true)};
    if (!lua) {
        return UnitSlots();
    }

    using GetTargets = std::function<sol::table(const bindings::UnitSlotView&,
                                                const bindings::UnitSlotView&, const UnitSlots&,
                                                const UnitSlots&, bool)>;
    auto getTargets = getScriptFunction<GetTargets>(*lua, "getTargets");
    if (!getTargets) {
        showErrorMessageBox(fmt::format("Could not find function 'getTargets' in script '{:s}'.\n"
                                        "Make sure function exists and has correct signature.",
                                        path.string()));
        return UnitSlots();
    }

    try {
        return (*getTargets)(attacker, selected, allies, targets, targetsAreAllies).as<UnitSlots>();
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                        "Reason: '{:s}'",
                                        path.string(), e.what()));
        return UnitSlots();
    }
}

UnitSlots getTargets(const game::IMidgardObjectMap* objectMap,
                     const game::BattleMsgData* battleMsgData,
                     const game::IBatAttack* batAttack,
                     const game::CMidgardID* targetGroupId,
                     const game::CMidgardID* unitId,
                     const game::CMidgardID* selectedUnitId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battle = BattleMsgDataApi::get();
    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    void* tmp{};
    auto targetGroup = fn.getStackFortRuinGroup(tmp, objectMap, targetGroupId);

    auto transformSelfAttack = (CBatAttackTransformSelf*)
        dynamicCast(batAttack, 0, rtti.IBatAttackType, rtti.CBatAttackTransformSelfType, 0);

    UnitSlots value;
    bool isSummonAttack = batAttack->vftable->method17(batAttack, battleMsgData);
    bool isAltAttack = transformSelfAttack != nullptr && *unitId != *selectedUnitId;
    for (size_t i = 0; i < std::size(targetGroup->positions); ++i) {
        CMidgardID targetUnitId = targetGroup->positions[i];
        if (targetUnitId != emptyId) {
            auto targetUnit = fn.findUnitById(objectMap, &targetUnitId);
            if (i % 2 && !isUnitSmall(targetUnit))
                continue;

            // Explicitly check if alt attack can be performed on self, because
            // canPerformAttackOnUnitWithStatusCheck always succeeds in case of transform-self
            // attack regardless of whether transform or alt-attack is going to be performed
            if (isAltAttack && targetUnitId == *unitId) {
                auto altAttack = transformSelfAttack->altAttack;
                if (altAttack != nullptr
                    && !altAttack->vftable->canPerform(altAttack, objectMap, battleMsgData,
                                                       &targetUnitId))
                    continue;
            }

            if (battle.canPerformAttackOnUnitWithStatusCheck(objectMap, battleMsgData, batAttack,
                                                             &targetUnitId)) {
                value.emplace_back(bindings::UnitSlotView(targetUnit, i, targetGroupId));
            }
        } else if (isSummonAttack) {
            value.emplace_back(bindings::UnitSlotView(nullptr, i, targetGroupId));
        }
    }

    return value;
}

std::vector<bindings::UnitSlotView> getAllies(const game::IMidgardObjectMap* objectMap,
                                              const game::BattleMsgData* battleMsgData,
                                              const game::CMidgardID* unitGroupId,
                                              const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battle = BattleMsgDataApi::get();

    void* tmp{};
    auto unitGroup = fn.getStackFortRuinGroup(tmp, objectMap, unitGroupId);

    UnitSlots value;
    for (size_t i = 0; i < std::size(unitGroup->positions); ++i) {
        CMidgardID allyUnitId = unitGroup->positions[i];
        if (allyUnitId == emptyId || allyUnitId == *unitId)
            continue;

        auto allyUnit = fn.findUnitById(objectMap, &allyUnitId);
        if (i % 2 && !isUnitSmall(allyUnit))
            continue;

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

    listApi.clear(value);

    void* tmp{};
    auto unitGroup = fn.getStackFortRuinGroup(tmp, objectMap, unitGroupId);

    auto unit = fn.findUnitById(objectMap, unitId);
    int unitPosition = groupApi.getUnitPosition(unitGroup, unitId);
    bindings::UnitSlotView attacker(unit, unitPosition, unitGroupId);

    bindings::UnitSlotView selected(nullptr, -1, &emptyId);

    auto targets = getTargets(objectMap, battleMsgData, batAttack, targetGroupId, unitId, &emptyId);
    auto allies = getAllies(objectMap, battleMsgData, unitGroupId, unitId);

    auto targetsToSelect = getTargetsToSelectOrAttack(attackReach.selectionScript, attacker,
                                                      selected, allies, targets,
                                                      *unitGroupId == *targetGroupId);

    bool isSummonAttack = batAttack->vftable->method17(batAttack, battleMsgData);
    for (const auto& target : targetsToSelect) {
        int position = target.getPosition();
        Pair<TargetsListIterator, bool> tmp{};
        listApi.insert(value, &tmp, &position);

        if (isSummonAttack && !(position % 2)) {
            auto unit = target.getUnit();
            if (unit && !isUnitSmall(unit)) {
                int backPosition = position + 1;
                listApi.insert(value, &tmp, &backPosition);
            }
        }
    }
}

void getTargetsToAttackForAllAttackReach(const game::IMidgardObjectMap* objectMap,
                                         const game::BattleMsgData* battleMsgData,
                                         const game::IAttack* attack,
                                         const game::IBatAttack* batAttack,
                                         const game::CMidgardID* targetGroupId,
                                         const game::CMidgardID* targetUnitId,
                                         game::IdList* value)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& id = CMidgardIDApi::get();
    const auto& listApi = IdListApi::get();
    const auto& groupApi = CMidUnitGroupApi::get();
    const auto& attackClasses = AttackClassCategories::get();

    void* tmp{};
    CMidUnitGroup* targetGroup = fn.getStackFortRuinGroup(tmp, objectMap, targetGroupId);

    CMidgardID summonUnitId{};
    id.isSummonUnitId(&summonUnitId, targetUnitId);

    if (summonUnitId != emptyId || groupApi.getUnitPosition(targetGroup, targetUnitId) != -1) {
        if (attack != nullptr
            && attack->vftable->getAttackClass(attack)->id == attackClasses.summon->id) {
            groupApi.addUnitIdsAvailableForSummons(value, objectMap, targetGroup);
        } else {
            const auto& unitIds = targetGroup->units;
            for (const CMidgardID* unitId = unitIds.bgn; unitId != unitIds.end; ++unitId) {
                listApi.pushBack(value, unitId);
            }
        }
    }

    addUniqueIdToList(*value, targetUnitId);
    listApi.shuffle(value);
}

UnitSlots getTargetsToAttackForCustomAttackReach(const game::IMidgardObjectMap* objectMap,
                                                 const game::BattleMsgData* battleMsgData,
                                                 const game::IBatAttack* batAttack,
                                                 const game::CMidgardID* targetGroupId,
                                                 const game::CMidgardID* targetUnitId,
                                                 const game::CMidgardID* unitGroupId,
                                                 const game::CMidgardID* unitId,
                                                 const CustomAttackReach& attackReach)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& id = CMidgardIDApi::get();
    const auto& groupApi = CMidUnitGroupApi::get();

    void* tmp{};
    auto unitGroup = fn.getStackFortRuinGroup(tmp, objectMap, unitGroupId);
    auto targetGroup = fn.getStackFortRuinGroup(tmp, objectMap, targetGroupId);

    auto unit = fn.findUnitById(objectMap, unitId);
    int unitPosition = groupApi.getUnitPosition(unitGroup, unitId);
    bindings::UnitSlotView attacker(unit, unitPosition, unitGroupId);

    auto target = fn.findUnitById(objectMap, targetUnitId);
    int targetPosition = groupApi.getUnitPosition(targetGroup, targetUnitId);
    if (targetPosition == -1) {
        targetPosition = id.summonUnitIdToPosition(targetUnitId);
    }
    bindings::UnitSlotView selected(target, targetPosition, targetGroupId);

    auto targets = getTargets(objectMap, battleMsgData, batAttack, targetGroupId, unitId,
                              targetUnitId);
    auto allies = getAllies(objectMap, battleMsgData, unitGroupId, unitId);

    return getTargetsToSelectOrAttack(attackReach.attackScript, attacker, selected, allies, targets,
                                      *unitGroupId == *targetGroupId);
}

UnitSlots getTargetsToAttackForCustomAttackReach(const game::IMidgardObjectMap* objectMap,
                                                 const game::BattleMsgData* battleMsgData,
                                                 const game::IAttack* attack,
                                                 const game::CMidgardID* targetGroupId,
                                                 const game::CMidgardID* targetUnitId,
                                                 const game::CMidgardID* unitGroupId,
                                                 const game::CMidgardID* unitId,
                                                 const game::CMidgardID* attackUnitId,
                                                 const CustomAttackReach& attackReach)
{
    using namespace game;

    const auto& fn = gameFunctions();

    const auto attackClass = attack->vftable->getAttackClass(attack);
    const auto batAttack = fn.createBatAttack(objectMap, battleMsgData, unitId, attackUnitId, 1,
                                              attackClass, false);

    auto result = getTargetsToAttackForCustomAttackReach(objectMap, battleMsgData, batAttack,
                                                         targetGroupId, targetUnitId, unitGroupId,
                                                         unitId, attackReach);

    batAttack->vftable->destructor(batAttack, true);
    return result;
}

void getTargetsToAttackForCustomAttackReach(const game::IMidgardObjectMap* objectMap,
                                            const game::BattleMsgData* battleMsgData,
                                            const game::IBatAttack* batAttack,
                                            const game::CMidgardID* targetGroupId,
                                            const game::CMidgardID* targetUnitId,
                                            const game::CMidgardID* unitGroupId,
                                            const game::CMidgardID* unitId,
                                            const CustomAttackReach& attackReach,
                                            game::IdList* value)
{
    using namespace game;

    const auto& id = CMidgardIDApi::get();

    auto targets = getTargetsToAttackForCustomAttackReach(objectMap, battleMsgData, batAttack,
                                                          targetGroupId, targetUnitId, unitGroupId,
                                                          unitId, attackReach);
    for (const auto& target : targets) {
        CMidgardID targetUnitId = target.getUnitId();
        if (targetUnitId == emptyId) {
            id.summonUnitIdFromPosition(&targetUnitId, target.getPosition());
        }
        addUniqueIdToList(*value, &targetUnitId);
    }
}

bool shouldExcludeImmuneTargets(const game::IMidgardObjectMap* objectMap,
                                const game::BattleMsgData* battleMsgData,
                                const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& id = CMidgardIDApi::get();
    const auto& battle = BattleMsgDataApi::get();

    if (battle.isAutoBattle(battleMsgData))
        return true;

    CMidgardID playerId{};
    id.validateId(&playerId, battle.isUnitAttacker(battleMsgData, unitId)
                                 ? battleMsgData->attackerPlayerId
                                 : battleMsgData->defenderPlayerId);
    if (playerId == emptyId)
        return true;

    auto player = getPlayer(objectMap, &playerId);
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

    void* tmp{};
    auto unitGroup = fn.getStackFortRuinGroup(tmp, objectMap, unitGroupId);
    auto targetGroup = fn.getStackFortRuinGroup(tmp, objectMap, targetGroupId);

    TargetsListIterator it, end;
    listApi.end(value, &end);
    for (listApi.begin(value, &it); !listApi.equals(&it, &end); listApi.preinc(&it)) {
        int targetPosition = *listApi.dereference(&it);

        auto unitId = getTargetUnitId(targetPosition, targetGroup, unitGroup);
        if (unitId == emptyId)
            continue;

        if (fn.isUnitImmuneToAttack(objectMap, battleMsgData, &unitId, attack, true)) {
            listApi.erase(value, &targetPosition);
        }
    }
}

void fillCustomDamageRatios(const game::IAttack* attack, const game::IdList* targets)
{
    using namespace game;

    const auto& listApi = IdListApi::get();

    auto ratios = computeAttackDamageRatio(attack, targets->length);
    if (ratios.empty())
        return;

    auto& customRatios = getCustomAttacks().damageRatios.value;
    auto ratioIt = ratios.begin();
    IdListIterator it, end;
    for (listApi.begin(targets, &it), listApi.end(targets, &end); !listApi.equals(&it, &end);
         listApi.preinc(&it)) {
        CMidgardID unitId = *listApi.dereference(&it);
        customRatios[unitId] = *(ratioIt++);
    }
}

int applyAttackDamageRatio(int damage, double ratio)
{
    if (damage == 0)
        return 0;

    int result = lround(ratio * damage);
    return result > 0 ? result : 1;
}

std::vector<double> computeAttackDamageRatio(const game::IAttack* attack, int targetCount)
{
    std::vector<double> result;

    auto attackImpl = getAttackImpl(attack);
    if (!attackImpl)
        return result;

    if (attackImpl->data->damageRatio == 100 && !attackImpl->data->damageSplit)
        return result;

    const double ratio = (double)attackImpl->data->damageRatio / 100;

    double currentRatio = ratio;
    double totalRatio = 1.0;
    result.push_back(1.0);
    for (int i = 1; i < targetCount; i++) {
        result.push_back(currentRatio);
        totalRatio += currentRatio;
        if (attackImpl->data->damageRatioPerTarget)
            currentRatio *= ratio;
    }

    if (attackImpl->data->damageSplit) {
        for (auto it = result.begin(); it != result.end(); ++it) {
            *it = *it / totalRatio * userSettings().splitDamageMultiplier;
        }
    }

    return result;
}

double computeTotalDamageRatio(const game::IAttack* attack, int targetCount)
{
    if (!getCustomAttacks().damageRatios.enabled)
        return targetCount;

    auto attackImpl = getAttackImpl(attack);
    if (!attackImpl)
        return targetCount;

    auto damageRatio = attackImpl->data->damageRatio;
    if (attackImpl->data->damageSplit) {
        return 1.0 * userSettings().splitDamageMultiplier;
    } else if (damageRatio != 100) {
        double ratio = (double)damageRatio / 100;

        double value = 1.0;
        for (int i = 1; i < targetCount; i++) {
            value += attackImpl->data->damageRatioPerTarget ? pow(ratio, i) : ratio;
        }

        return value;
    }

    return targetCount;
}

int computeAverageTotalDamage(const game::IAttack* attack, int damage)
{
    using namespace game;

    const auto& reaches = AttackReachCategories::get();

    auto attackReach = attack->vftable->getAttackReach(attack);
    if (attackReach->id == reaches.all->id) {
        return damage * 3;
    } else if (attackReach->id != reaches.any->id && attackReach->id != reaches.adjacent->id) {
        for (const auto& custom : getCustomAttacks().reaches) {
            if (attackReach->id == custom.reach.id) {
                if (custom.maxTargets == 1)
                    return damage;
                return damage * custom.maxTargets / 2;
            }
        }
    }

    return damage;
}

} // namespace hooks
