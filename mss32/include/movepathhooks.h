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

#ifndef MOVEPATHHOOKS_H
#define MOVEPATHHOOKS_H

#include "d2list.h"
#include "mqpoint.h"

namespace game {
struct IMidgardObjectMap;
struct CMidgardID;
struct CMidgardMap;
struct CMidgardPlan;
} // namespace game

namespace hooks {

void __stdcall showMovementPathHooked(const game::IMidgardObjectMap* objectMap,
                                      const game::CMidgardID* stackId,
                                      game::List<game::CMqPoint>* path,
                                      const game::CMqPoint* lastReachablePoint,
                                      const game::CMqPoint* pathEnd,
                                      bool a6);

int __stdcall computeMovementCostHooked(const game::CMqPoint* mapPosition,
                                        const game::IMidgardObjectMap* objectMap,
                                        const game::CMidgardMap* midgardMap,
                                        const game::CMidgardPlan* plan,
                                        const game::CMidgardID* stackId,
                                        const char* a6,
                                        const char* a7,
                                        bool leaderAlive,
                                        bool plainsBonus,
                                        bool forestBonus,
                                        bool waterBonus,
                                        bool waterOnly,
                                        bool forbidWaterOnlyOnLand);

} // namespace hooks

#endif // MOVEPATHHOOKS_H
