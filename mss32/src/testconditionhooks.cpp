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

#include "testconditionhooks.h"
#include "eventconditioncathooks.h"
#include "midcondgamemode.h"
#include "midcondownresource.h"
#include "midcondplayertype.h"
#include "midcondvarcmp.h"
#include "originalfunctions.h"

namespace hooks {

game::ITestCondition* __stdcall createTestConditionHooked(game::CMidEvCondition* eventCondition,
                                                          bool samePlayer,
                                                          const game::CMidgardID* triggererStackId)
{
    const auto& conditions = customEventConditions();
    const auto id = eventCondition->category.id;

    if (id == conditions.ownResource.category.id) {
        return createTestOwnResource(eventCondition);
    }

    if (id == conditions.gameMode.category.id) {
        return createTestGameMode(eventCondition);
    }

    if (id == conditions.playerType.category.id) {
        return createTestPlayerType(eventCondition);
    }

    if (id == conditions.variableCmp.category.id) {
        return createTestVarCmp(eventCondition);
    }

    return getOriginalFunctions().createTestCondition(eventCondition, samePlayer, triggererStackId);
}

} // namespace hooks
