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

#include "mideventhooks.h"
#include "eventconditioncathooks.h"
#include "midcondgamemode.h"
#include "midcondownresource.h"
#include "midevent.h"
#include "originalfunctions.h"

namespace hooks {

void getConditionsOfType(const game::CMidEvent* event,
                         const game::LEventCondCategory* category,
                         std::vector<const game::CMidEvCondition*>& conditions)
{
    const int conditionsTotal = event->conditions.end - event->conditions.bgn;

    for (int i = 0; i < conditionsTotal; ++i) {
        const auto condition = event->conditions.bgn[i];

        if (condition->category.id == category->id) {
            conditions.push_back(condition);
        }
    }
}

bool __stdcall checkEventValidHooked(game::CDialogInterf* dialog,
                                     const game::IMidgardObjectMap* objectMap,
                                     const game::CMidgardID* eventId)
{
    if (!getOriginalFunctions().checkEventValid(dialog, objectMap, eventId)) {
        return false;
    }

    return checkOwnResourceConditionsValid(dialog, objectMap, eventId)
           && checkGameModeConditionValid(dialog, objectMap, eventId);
}

} // namespace hooks
