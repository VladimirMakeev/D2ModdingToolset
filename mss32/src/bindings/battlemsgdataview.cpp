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
#include "battlemsgdata.h"
#include "gameutils.h"
#include "idview.h"
#include "playerview.h"
#include "stackview.h"
#include <sol/sol.hpp>

namespace bindings {

BattleMsgDataView::BattleMsgDataView(const game::BattleMsgData* battleMsgData,
                                     const game::IMidgardObjectMap* objectMap)
    : battleMsgData{battleMsgData}
    , objectMap{objectMap}
{ }

void BattleMsgDataView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<BattleMsgDataView>("BattleView");
    view["getUnitStatus"] = &BattleMsgDataView::getUnitStatus;
    view["currentRound"] = sol::property(&BattleMsgDataView::getCurrentRound);
    view["autoBattle"] = sol::property(&BattleMsgDataView::getAutoBattle);
    view["attackerPlayer"] = sol::property(&BattleMsgDataView::getAttackerPlayer);
    view["defenderPlayer"] = sol::property(&BattleMsgDataView::getDefenderPlayer);
    view["attacker"] = sol::property(&BattleMsgDataView::getAttacker);
    view["defender"] = sol::property(&BattleMsgDataView::getDefender);
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

std::optional<PlayerView> BattleMsgDataView::getPlayer(const game::CMidgardID& playerId) const
{
    auto player{hooks::getPlayer(objectMap, &playerId)};
    if (!player) {
        return std::nullopt;
    }

    return PlayerView{player};
}

} // namespace bindings
