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

#include "battlemsgdataview.h"
#include "attackclasscat.h"
#include "battlemsgdata.h"
#include "customattacks.h"
#include "game.h"
#include "gameutils.h"
#include "idview.h"
#include "playerview.h"
#include "stackview.h"
#include <sol/sol.hpp>

namespace bindings {

BattleTurnView::BattleTurnView(const game::CMidgardID& unitId,
                               char attackCount,
                               const game::IMidgardObjectMap* objectMap)
    : unitId{unitId}
    , objectMap{objectMap}
    , attackCount{attackCount}
{ }

void BattleTurnView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<BattleTurnView>("BattleTurnView");
    view["unit"] = sol::property(&BattleTurnView::getUnit);
    view["attackCount"] = sol::property(&BattleTurnView::getAttackCount);
}

UnitView BattleTurnView::getUnit() const
{
    return UnitView{game::gameFunctions().findUnitById(objectMap, &unitId)};
}

int BattleTurnView::getAttackCount() const
{
    return attackCount;
}

BattleMsgDataView::BattleMsgDataView(const game::BattleMsgData* battleMsgData,
                                     const game::IMidgardObjectMap* objectMap)
    : battleMsgData{battleMsgData}
    , objectMap{objectMap}
{ }

void BattleMsgDataView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<BattleMsgDataView>("BattleView");
    bindAccessMethods(view);
}

bool BattleMsgDataView::getUnitStatus(const IdView& unitId, int status) const
{
    using namespace game;

    return BattleMsgDataApi::get().getUnitStatus(battleMsgData, &unitId.id, (BattleStatus)status);
}

int BattleMsgDataView::getCurrentRound() const
{
    return battleMsgData->currentRound;
}

bool BattleMsgDataView::getAutoBattle() const
{
    return game::BattleMsgDataApi::get().isAutoBattle(battleMsgData);
}

bool BattleMsgDataView::getFastBattle() const
{
    return game::BattleMsgDataApi::get().isFastBattle(battleMsgData);
}

std::optional<PlayerView> BattleMsgDataView::getAttackerPlayer() const
{
    return getPlayer(battleMsgData->attackerPlayerId);
}

std::optional<PlayerView> BattleMsgDataView::getDefenderPlayer() const
{
    return getPlayer(battleMsgData->defenderPlayerId);
}

std::optional<StackView> BattleMsgDataView::getAttacker() const
{
    auto stack{hooks::getStack(objectMap, &battleMsgData->attackerGroupId)};
    if (!stack) {
        return std::nullopt;
    }

    return StackView{stack, objectMap};
}

std::optional<GroupView> BattleMsgDataView::getDefender() const
{
    auto group{hooks::getGroup(objectMap, &battleMsgData->defenderGroupId)};
    if (!group) {
        return std::nullopt;
    }

    return GroupView{group, objectMap, &battleMsgData->defenderGroupId};
}

game::RetreatStatus BattleMsgDataView::getRetreatStatus(bool attacker) const
{
    return game::BattleMsgDataApi::get().getRetreatStatus(battleMsgData, attacker);
}

bool BattleMsgDataView::isRetreatDecisionWasMade() const
{
    return game::BattleMsgDataApi::get().isRetreatDecisionWasMade(battleMsgData);
}

bool BattleMsgDataView::isUnitAttacker(const UnitView& unit) const
{
    return isUnitAttackerId(unit.getId());
}

bool BattleMsgDataView::isUnitAttackerId(const IdView& unitId) const
{
    return game::BattleMsgDataApi::get().isUnitAttacker(battleMsgData, &unitId.id);
}

bool BattleMsgDataView::isAfterBattle() const
{
    return game::BattleMsgDataApi::get().isAfterBattle(battleMsgData);
}

BattleMsgDataView::UnitActions BattleMsgDataView::getUnitActions(const UnitView& unit) const
{
    return getUnitActionsById(unit.getId());
}

BattleMsgDataView::UnitActions BattleMsgDataView::getUnitActionsById(const IdView& unitId) const
{
    using namespace game;

    std::vector<BattleAction> actions;
    std::optional<GroupView> attackTargetGroup;
    std::vector<int> attackTargets;
    std::optional<GroupView> item1TargetGroup;
    std::vector<int> item1Targets;
    std::optional<GroupView> item2TargetGroup;
    std::vector<int> item2Targets;

    {
        const auto& setApi = IntSetApi::get();

        Set<BattleAction> battleActions;
        setApi.constructor((IntSet*)&battleActions);

        GroupIdTargetsPair attackTargetsPair{};
        setApi.constructor(&attackTargetsPair.second);

        GroupIdTargetsPair item1TargetsPair{};
        setApi.constructor(&item1TargetsPair.second);

        GroupIdTargetsPair item2TargetsPair{};
        setApi.constructor(&item2TargetsPair.second);

        game::BattleMsgDataApi::get().updateBattleActions(objectMap, battleMsgData, &unitId.id,
                                                          &battleActions, &attackTargetsPair,
                                                          &item1TargetsPair, &item2TargetsPair);

        // Copy data into std containers for sol2
        {
            actions.reserve(battleActions.length);
            for (auto& v : battleActions) {
                actions.push_back(v);
            }

            if (const auto* group = hooks::getGroup(objectMap, &attackTargetsPair.first)) {
                attackTargetGroup = bindings::GroupView{group, objectMap, &attackTargetsPair.first};
            }

            attackTargets.reserve(attackTargetsPair.second.length);
            for (auto& v : attackTargetsPair.second) {
                attackTargets.push_back(v);
            }

            if (const auto* group = hooks::getGroup(objectMap, &item1TargetsPair.first)) {
                item1TargetGroup = bindings::GroupView{group, objectMap, &item1TargetsPair.first};
            }

            item1Targets.reserve(item1TargetsPair.second.length);
            for (auto& v : item1TargetsPair.second) {
                item1Targets.push_back(v);
            }

            if (const auto* group = hooks::getGroup(objectMap, &item2TargetsPair.first)) {
                item2TargetGroup = bindings::GroupView{group, objectMap, &item2TargetsPair.first};
            }

            item2Targets.reserve(item2TargetsPair.second.length);
            for (auto& v : item2TargetsPair.second) {
                item2Targets.push_back(v);
            }
        }

        setApi.destructor(&item2TargetsPair.second);
        setApi.destructor(&item1TargetsPair.second);
        setApi.destructor(&attackTargetsPair.second);
        setApi.destructor((IntSet*)&battleActions);
    }

    return {actions,                          //
            attackTargetGroup, attackTargets, //
            item1TargetGroup,  item1Targets,  //
            item2TargetGroup,  item2Targets};
}

int BattleMsgDataView::getUnitShatteredArmor(const UnitView& unit) const
{
    return getUnitShatteredArmorById(unit.getId());
}

int BattleMsgDataView::getUnitShatteredArmorById(const IdView& unitId) const
{
    return game::BattleMsgDataApi::get().getUnitShatteredArmor(battleMsgData, &unitId.id);
}

int BattleMsgDataView::getUnitFortificationArmor(const UnitView& unit) const
{
    return getUnitFortificationArmorById(unit.getId());
}

int BattleMsgDataView::getUnitFortificationArmorById(const IdView& unitId) const
{
    return game::BattleMsgDataApi::get().getUnitFortificationArmor(battleMsgData, &unitId.id);
}

bool BattleMsgDataView::isUnitResistantToSource(const UnitView& unit, int sourceId) const
{
    return isUnitResistantToSourceById(unit.getId(), sourceId);
}

bool BattleMsgDataView::isUnitResistantToSourceById(const IdView& unitId, int sourceId) const
{
    using namespace game;

    const auto& attackSources{AttackSourceCategories::get()};
    const LAttackSource* source = nullptr;

    const AttackSourceId srcId{static_cast<AttackSourceId>(sourceId)};
    switch (srcId) {
    default: {
        // Check custom attack sources first
        const auto& customSources = hooks::getCustomAttacks().sources;
        for (const auto& customSource : customSources) {
            if (customSource.source.id == srcId) {
                source = &customSource.source;
                break;
            }
        }

        if (source == nullptr) {
            // Could not find source id even in custom sources
            return false;
        }

        break;
    }
    case AttackSourceId::Weapon:
        source = attackSources.weapon;
        break;
    case AttackSourceId::Mind:
        source = attackSources.mind;
        break;
    case AttackSourceId::Life:
        source = attackSources.life;
        break;
    case AttackSourceId::Death:
        source = attackSources.death;
        break;
    case AttackSourceId::Fire:
        source = attackSources.fire;
        break;
    case AttackSourceId::Water:
        source = attackSources.water;
        break;
    case AttackSourceId::Earth:
        source = attackSources.earth;
        break;
    case AttackSourceId::Air:
        source = attackSources.air;
        break;
    }

    return !BattleMsgDataApi::get().isUnitAttackSourceWardRemoved(battleMsgData, &unitId.id,
                                                                  source);
}

bool BattleMsgDataView::isUnitResistantToClass(const UnitView& unit, int classId) const
{
    return isUnitResistantToClassById(unit.getId(), classId);
}

bool BattleMsgDataView::isUnitResistantToClassById(const IdView& unitId, int classId) const
{
    using namespace game;

    const auto& attackClasses{AttackClassCategories::get()};
    const LAttackClass* attackClass = nullptr;

    const AttackClassId id{static_cast<AttackClassId>(classId)};
    switch (id) {
    case AttackClassId::Damage:
        attackClass = attackClasses.damage;
        break;
    case AttackClassId::Drain:
        attackClass = attackClasses.drain;
        break;
    case AttackClassId::Paralyze:
        attackClass = attackClasses.paralyze;
        break;
    case AttackClassId::Heal:
        attackClass = attackClasses.heal;
        break;
    case AttackClassId::Fear:
        attackClass = attackClasses.fear;
        break;
    case AttackClassId::BoostDamage:
        attackClass = attackClasses.boostDamage;
        break;
    case AttackClassId::Petrify:
        attackClass = attackClasses.petrify;
        break;
    case AttackClassId::LowerDamage:
        attackClass = attackClasses.lowerDamage;
        break;
    case AttackClassId::LowerInitiative:
        attackClass = attackClasses.lowerInitiative;
        break;
    case AttackClassId::Poison:
        attackClass = attackClasses.poison;
        break;
    case AttackClassId::Frostbite:
        attackClass = attackClasses.frostbite;
        break;
    case AttackClassId::Revive:
        attackClass = attackClasses.revive;
        break;
    case AttackClassId::DrainOverflow:
        attackClass = attackClasses.drainOverflow;
        break;
    case AttackClassId::Cure:
        attackClass = attackClasses.cure;
        break;
    case AttackClassId::Summon:
        attackClass = attackClasses.summon;
        break;
    case AttackClassId::DrainLevel:
        attackClass = attackClasses.drainLevel;
        break;
    case AttackClassId::GiveAttack:
        attackClass = attackClasses.giveAttack;
        break;
    case AttackClassId::Doppelganger:
        attackClass = attackClasses.doppelganger;
        break;
    case AttackClassId::TransformSelf:
        attackClass = attackClasses.transformSelf;
        break;
    case AttackClassId::TransformOther:
        attackClass = attackClasses.transformOther;
        break;
    case AttackClassId::Blister:
        attackClass = attackClasses.blister;
        break;
    case AttackClassId::BestowWards:
        attackClass = attackClasses.bestowWards;
        break;
    case AttackClassId::Shatter:
        attackClass = attackClasses.shatter;
        break;
    default:
        return false;
    }

    return BattleMsgDataApi::get().isUnitAttackClassWardRemoved(battleMsgData, &unitId.id,
                                                                attackClass);
}

std::vector<BattleTurnView> BattleMsgDataView::getTurnsOrder() const
{
    std::vector<BattleTurnView> turns;

    for (const auto& battleTurn : battleMsgData->turnsOrder) {
        if (battleTurn.unitId == game::invalidId) {
            break;
        }

        turns.emplace_back(battleTurn.unitId, battleTurn.attackCount, objectMap);
    }

    return turns;
}

std::optional<PlayerView> BattleMsgDataView::getPlayer(const game::CMidgardID& playerId) const
{
    auto player{hooks::getPlayer(objectMap, &playerId)};
    if (!player) {
        return std::nullopt;
    }

    return PlayerView{player, objectMap};
}

} // namespace bindings
