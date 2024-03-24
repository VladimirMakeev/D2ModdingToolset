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

#ifndef BATTLEMSGDATAVIEW_H
#define BATTLEMSGDATAVIEW_H

#include "unitview.h"
#include <optional>
#include <tuple>
#include <vector>

namespace sol {
class state;
}

namespace game {
struct BattleMsgData;
struct IMidgardObjectMap;
struct CMidgardID;
enum class BattleAction : int;
enum class RetreatStatus : std::uint8_t;
} // namespace game

namespace bindings {

struct IdView;
class PlayerView;
class StackView;
class GroupView;

class BattleTurnView
{
public:
    BattleTurnView(const game::CMidgardID& unitId,
                   char attackCount,
                   const game::IMidgardObjectMap* objectMap);

    static void bind(sol::state& lua);

    UnitView getUnit() const;
    int getAttackCount() const;

private:
    game::CMidgardID unitId;
    const game::IMidgardObjectMap* objectMap;
    char attackCount;
};

class BattleMsgDataView
{
public:
    BattleMsgDataView(const game::BattleMsgData* battleMsgData,
                      const game::IMidgardObjectMap* objectMap);

    static void bind(sol::state& lua);

    bool getUnitStatus(const IdView& unitId, int status) const;

    int getCurrentRound() const;
    bool getAutoBattle() const;
    bool getFastBattle() const;

    std::optional<PlayerView> getAttackerPlayer() const;
    std::optional<PlayerView> getDefenderPlayer() const;

    std::optional<StackView> getAttacker() const;
    std::optional<GroupView> getDefender() const;

    game::RetreatStatus getRetreatStatus(bool attacker) const;
    bool isRetreatDecisionWasMade() const;

    bool isUnitAttacker(const UnitView& unit) const;
    bool isUnitAttackerId(const IdView& unitId) const;
    bool isAfterBattle() const;
    bool isDuel() const;

    using UnitActions = std::tuple<std::vector<game::BattleAction>, // Possible unit actions
                                   std::optional<GroupView>,        // Attack target group
                                   std::vector<int>,                // Attack targets
                                   std::optional<GroupView>,        // Item 1 target group
                                   std::vector<int>,                // Item 1 targets
                                   std::optional<GroupView>,        // Item 2 target group
                                   std::vector<int>>;               // Item 2 targets

    UnitActions getUnitActions(const UnitView& unit) const;
    UnitActions getUnitActionsById(const IdView& unitId) const;

    int getUnitShatteredArmor(const UnitView& unit) const;
    int getUnitShatteredArmorById(const IdView& unitId) const;

    int getUnitFortificationArmor(const UnitView& unit) const;
    int getUnitFortificationArmorById(const IdView& unitId) const;

    bool isUnitResistantToSource(const UnitView& unit, int sourceId) const;
    bool isUnitResistantToSourceById(const IdView& unitId, int sourceId) const;

    bool isUnitResistantToClass(const UnitView& unit, int classId) const;
    bool isUnitResistantToClassById(const IdView& unitId, int classId) const;

    std::vector<BattleTurnView> getTurnsOrder() const;

    bool isUnitRevived(const UnitView& unit) const;
    bool isUnitRevivedById(const IdView& unitId) const;

    int getUnitDisableRound(const UnitView& unit) const;
    int getUnitDisableRoundById(const IdView& unitId) const;

    int getUnitPoisonRound(const UnitView& unit) const;
    int getUnitPoisonRoundById(const IdView& unitId) const;

    int getUnitFrostbiteRound(const UnitView& unit) const;
    int getUnitFrostbiteRoundById(const IdView& unitId) const;

    int getUnitBlisterRound(const UnitView& unit) const;
    int getUnitBlisterRoundById(const IdView& unitId) const;

    int getUnitTransformRound(const UnitView& unit) const;
    int getUnitTransformRoundById(const IdView& unitId) const;

protected:
    template <typename T>
    static void bindAccessMethods(T& view)
    {
        view["getUnitStatus"] = &BattleMsgDataView::getUnitStatus;
        view["currentRound"] = sol::property(&BattleMsgDataView::getCurrentRound);
        view["autoBattle"] = sol::property(&BattleMsgDataView::getAutoBattle);
        view["fastBattle"] = sol::property(&BattleMsgDataView::getFastBattle);
        view["attackerPlayer"] = sol::property(&BattleMsgDataView::getAttackerPlayer);
        view["defenderPlayer"] = sol::property(&BattleMsgDataView::getDefenderPlayer);
        view["attacker"] = sol::property(&BattleMsgDataView::getAttacker);
        view["defender"] = sol::property(&BattleMsgDataView::getDefender);
        view["isUnitAttacker"] = sol::overload<>(&BattleMsgDataView::isUnitAttacker,
                                                 &BattleMsgDataView::isUnitAttackerId);
        view["getUnitActions"] = sol::overload<>(&BattleMsgDataView::getUnitActions,
                                                 &BattleMsgDataView::getUnitActionsById);
        view["getRetreatStatus"] = &BattleMsgDataView::getRetreatStatus;
        view["decidedToRetreat"] = sol::property(&BattleMsgDataView::isRetreatDecisionWasMade);
        view["afterBattle"] = sol::property(&BattleMsgDataView::isAfterBattle);
        view["duel"] = sol::property(&BattleMsgDataView::isDuel);
        view["turnsOrder"] = sol::property(&BattleMsgDataView::getTurnsOrder);
        view["isUnitRevived"] = sol::overload<>(&BattleMsgDataView::isUnitRevived,
                                                &BattleMsgDataView::isUnitRevivedById);

        view["getUnitShatteredArmor"] = sol::overload<>(
            &BattleMsgDataView::getUnitShatteredArmor,
            &BattleMsgDataView::getUnitShatteredArmorById);

        view["getUnitFortificationArmor"] = sol::overload<>(
            &BattleMsgDataView::getUnitFortificationArmor,
            &BattleMsgDataView::getUnitFortificationArmorById);

        view["isUnitResistantToSource"] = sol::overload<>(
            &BattleMsgDataView::isUnitResistantToSource,
            &BattleMsgDataView::isUnitResistantToSourceById);

        view["isUnitResistantToClass"] = sol::overload<>(
            &BattleMsgDataView::isUnitResistantToClass,
            &BattleMsgDataView::isUnitResistantToClassById);

        view["getUnitDisableRound"] = sol::overload<>(&BattleMsgDataView::getUnitDisableRound,
                                                      &BattleMsgDataView::getUnitDisableRoundById);

        view["getUnitPoisonRound"] = sol::overload<>(&BattleMsgDataView::getUnitPoisonRound,
                                                     &BattleMsgDataView::getUnitPoisonRoundById);

        view["getUnitFrostbiteRound"] = sol::overload<>(
            &BattleMsgDataView::getUnitFrostbiteRound,
            &BattleMsgDataView::getUnitFrostbiteRoundById);

        view["getUnitBlisterRound"] = sol::overload<>(&BattleMsgDataView::getUnitBlisterRound,
                                                      &BattleMsgDataView::getUnitBlisterRoundById);

        view["getUnitTransformRound"] = sol::overload<>(
            &BattleMsgDataView::getUnitTransformRound,
            &BattleMsgDataView::getUnitTransformRoundById);
    }

private:
    std::optional<PlayerView> getPlayer(const game::CMidgardID& playerId) const;

    const game::BattleMsgData* battleMsgData;
    const game::IMidgardObjectMap* objectMap;
};

} // namespace bindings

#endif // BATTLEMSGDATAVIEW_H
