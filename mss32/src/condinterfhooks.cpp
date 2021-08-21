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

#include "condinterfhooks.h"
#include "eventconditioncathooks.h"
#include "midcondgamemode.h"
#include "midcondownresource.h"
#include "midcondplayertype.h"
#include "originalfunctions.h"

namespace hooks {

game::editor::CCondInterf* __stdcall createCondInterfFromCategoryHooked(
    game::ITask* task,
    void* a2,
    const game::CMidgardID* eventId,
    const game::LEventCondCategory* category)
{
    const auto& conditions = customEventConditions();
    const auto id = category->id;

    if (id == conditions.ownResource.category.id) {
        return createCondOwnResourceInterf(task, a2, eventId);
    }

    if (id == conditions.gameMode.category.id) {
        return createCondGameModeInterf(task, a2, eventId);
    }

    if (id == conditions.playerType.category.id) {
        return createCondPlayerTypeInterf(task, a2, eventId);
    }

    return getOriginalFunctions().createCondInterfFromCategory(task, a2, eventId, category);
}

} // namespace hooks
