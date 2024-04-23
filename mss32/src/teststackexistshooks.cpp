/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#include "teststackexistshooks.h"
#include "eventconditions.h"
#include "gameutils.h"
#include "stacktemplatecache.h"
#include "teststackexists.h"
#include "timer.h"

namespace hooks {

bool __fastcall testStackExistsHooked(const game::CTestStackExists* thisptr,
                                      int /*%edx*/,
                                      const game::IMidgardObjectMap* objectMap,
                                      const game::CMidgardID* playerId,
                                      const game::CMidgardID* eventId)
{
    using namespace game;

#ifdef D2_MEASURE_EVENTS_TIME
    extern const std::string_view eventsPerformanceLog;
    ScopedTimer timer{"    Test condition 'stack exists'", eventsPerformanceLog};

    extern long long conditionsTotalTime;
    ScopedValueTimer valueTimer{conditionsTotalTime};
#endif

    const CMidgardID* stackId{&thisptr->condStackExists->stackId};

    if (CMidgardIDApi::get().getType(stackId) == IdType::Stack) {
        const CMidStack* stack{getStack(objectMap, stackId)};

        if (thisptr->condStackExists->existanceStatus) {
            return stack == nullptr;
        }

        return stack != nullptr;
    }

    // Stack id belongs to stack template id
    const bool stackCreatedFromTemplate{stackTemplateCacheCheck(*stackId)};
    if (stackCreatedFromTemplate) {
        if (thisptr->condStackExists->existanceStatus) {
            return false;
        }

        return true;
    }

    if (!thisptr->condStackExists->existanceStatus) {
        return false;
    }

    return true;
}

} // namespace hooks
