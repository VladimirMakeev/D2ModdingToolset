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

#ifndef BATTLEMSGDATAHOOKS_H
#define BATTLEMSGDATAHOOKS_H

#include "d2pair.h"
#include "d2set.h"

namespace game {
struct CMidgardID;
struct BattleMsgData;
struct IMidgardObjectMap;

using TargetSet = Set<int>;
using GroupIdTargetsPair = Pair<CMidgardID, TargetSet>;

enum class BattleAction : int;
} // namespace game

namespace hooks {

game::BattleMsgData* __fastcall battleMsgDataCtorHooked(game::BattleMsgData* thisptr, int /*%edx*/);

game::BattleMsgData* __fastcall battleMsgDataCopyCtorHooked(game::BattleMsgData* thisptr,
                                                            int /*%edx*/,
                                                            const game::BattleMsgData* src);

game::BattleMsgData* __fastcall battleMsgDataCopyAssignHooked(game::BattleMsgData* thisptr,
                                                              int /*%edx*/,
                                                              const game::BattleMsgData* src);

game::BattleMsgData* __fastcall battleMsgDataCopyHooked(game::BattleMsgData* thisptr,
                                                        int /*%edx*/,
                                                        const game::BattleMsgData* src);

void __fastcall battleMsgDataDtorHooked(game::BattleMsgData* thisptr, int /*%edx*/);

void __fastcall removeUnitInfoHooked(game::BattleMsgData* thisptr,
                                     int /*%edx*/,
                                     const game::CMidgardID* unitId);

void __stdcall updateBattleActionsHooked(const game::IMidgardObjectMap* objectMap,
                                         const game::BattleMsgData* battleMsgData,
                                         const game::CMidgardID* unitId,
                                         game::Set<game::BattleAction>* actions,
                                         game::GroupIdTargetsPair* attackTargets,
                                         game::GroupIdTargetsPair* item1Targets,
                                         game::GroupIdTargetsPair* item2Targets);

} // namespace hooks

#endif // BATTLEMSGDATAHOOKS_H
