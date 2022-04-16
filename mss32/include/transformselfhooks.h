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

#ifndef TRANSFORMSELFHOOKS_H
#define TRANSFORMSELFHOOKS_H

#include "targetset.h"

namespace game {
struct CBatAttackTransformSelf;
struct IMidgardObjectMap;
struct BattleMsgData;
struct CMidgardID;
struct BattleAttackInfo;
} // namespace game

namespace hooks {

void __fastcall transformSelfAttackOnHitHooked(game::CBatAttackTransformSelf* thisptr,
                                               int /*%edx*/,
                                               game::IMidgardObjectMap* objectMap,
                                               game::BattleMsgData* battleMsgData,
                                               game::CMidgardID* targetUnitId,
                                               game::BattleAttackInfo** attackInfo);

void __fastcall transformSelfAttackFillTargetsListHooked(game::CBatAttackTransformSelf* thisptr,
                                                         int /*%edx*/,
                                                         game::IMidgardObjectMap* objectMap,
                                                         game::BattleMsgData* battleMsgData,
                                                         game::TargetSet* targetsList);

} // namespace hooks

#endif // TRANSFORMSELFHOOKS_H
