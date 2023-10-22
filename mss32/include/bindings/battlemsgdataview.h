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

#include <optional>

namespace sol {
class state;
}

namespace game {
struct BattleMsgData;
struct IMidgardObjectMap;
struct CMidgardID;
} // namespace game

namespace bindings {

struct IdView;
class PlayerView;
class StackView;
class GroupView;

class BattleMsgDataView
{
public:
    BattleMsgDataView(const game::BattleMsgData* battleMsgData,
                      const game::IMidgardObjectMap* objectMap);

    static void bind(sol::state& lua);

    bool getUnitStatus(const IdView& unitId, int status) const;

    int getCurrentRound() const;
    bool getAutoBattle() const;

    std::optional<PlayerView> getAttackerPlayer() const;
    std::optional<PlayerView> getDefenderPlayer() const;

    std::optional<StackView> getAttacker() const;
    std::optional<GroupView> getDefender() const;

private:
    std::optional<PlayerView> getPlayer(const game::CMidgardID& playerId) const;

    const game::BattleMsgData* battleMsgData;
    const game::IMidgardObjectMap* objectMap;
};

} // namespace bindings

#endif // BATTLEMSGDATAVIEW_H
