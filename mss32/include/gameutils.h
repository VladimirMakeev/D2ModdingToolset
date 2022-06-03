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

#ifndef GAMEUTILS_H
#define GAMEUTILS_H

namespace game {
struct CMidgardID;
struct CMidUnitGroup;
struct IMidgardObjectMap;
struct BattleMsgData;
struct CScenarioInfo;
struct CMidPlayer;
struct CMidScenVariables;
struct CMidgardPlan;
struct CMidStack;
} // namespace game

namespace hooks {

bool isGreaterPickRandomIfEqual(int first, int second);

const game::CMidUnitGroup* getAllyOrEnemyGroup(const game::IMidgardObjectMap* objectMap,
                                               const game::BattleMsgData* battleMsgData,
                                               const game::CMidgardID* unitId,
                                               bool ally);

const game::CScenarioInfo* getScenarioInfo(const game::IMidgardObjectMap* objectMap);

const game::CMidPlayer* getPlayer(const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* playerId);

const game::CMidPlayer* getPlayer(const game::IMidgardObjectMap* objectMap,
                                  const game::BattleMsgData* battleMsgData,
                                  const game::CMidgardID* unitId);

const game::CMidScenVariables* getScenarioVariables(const game::IMidgardObjectMap* objectMap);

const game::CMidgardPlan* getMidgardPlan(const game::IMidgardObjectMap* objectMap);

game::CMidStack* getStack(const game::IMidgardObjectMap* objectMap,
                          const game::CMidgardID* stackId);

game::CMidStack* getStack(const game::IMidgardObjectMap* objectMap,
                          const game::BattleMsgData* battleMsgData,
                          const game::CMidgardID* unitId);

} // namespace hooks

#endif // GAMEUTILS_H
