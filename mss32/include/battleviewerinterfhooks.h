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

#ifndef BATTLEVIEWERINTERFHOOKS_H
#define BATTLEVIEWERINTERFHOOKS_H

#include "sortedlist.h"

namespace game {
struct CBattleViewerInterf;
struct CMqPoint;
struct IBatViewer;
struct BattleMsgData;
struct CMidgardID;
struct BatViewerTargetDataSet;

enum class BattleAction : int;
} // namespace game

namespace hooks {

bool __fastcall markAttackTargetsHooked(game::CBattleViewerInterf* thisptr,
                                        int /*%edx*/,
                                        const game::CMqPoint* mousePosition,
                                        bool setBigFace);

void __fastcall battleViewerInterfUpdateHooked(game::IBatViewer* thisptr,
                                               int /*%edx*/,
                                               const game::BattleMsgData* battleMsgData,
                                               const game::CMidgardID* unitId,
                                               const game::SortedList<game::BattleAction>* actions,
                                               const game::BatViewerTargetDataSet* targetData);

} // namespace hooks

#endif // BATTLEVIEWERINTERFHOOKS_H
