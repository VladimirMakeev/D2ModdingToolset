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

#include "testleadertozonehooks.h"
#include "dynamiccast.h"
#include "eventconditions.h"
#include "gameutils.h"
#include "midgardobjectmap.h"
#include "midgardplan.h"
#include "midlocation.h"
#include "midstack.h"
#include "mqrect.h"
#include "testleadertozone.h"
#include "timer.h"

namespace hooks {

static bool isStackInsideLocation(const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* stackId,
                                  const game::CMidgardID* locationId)
{
    using namespace game;

    const CMidStack* stack{getStack(objectMap, stackId)};
    if (!stack || stack->insideId != emptyId) {
        return false;
    }

    const CMidLocation* location{getLocation(objectMap, locationId)};
    const CMqPoint& pos{location->position};
    const int radius{location->radius};

    const int startX{pos.x - radius};
    const int startY{pos.y - radius};
    const int endX{pos.x + radius + 1};
    const int endY{pos.y + radius + 1};
    const CMqRect area{startX, startY, endX, endY};

    return MqRectApi::get().ptInRect(&area, &stack->position);
}

bool __fastcall testLeaderToZoneHooked(const game::CTestLeaderToZone* thisptr,
                                       int /*%edx*/,
                                       const game::IMidgardObjectMap* objectMap,
                                       const game::CMidgardID* playerId,
                                       const game::CMidgardID* eventId)
{
    using namespace game;

#ifdef D2_MEASURE_EVENTS_TIME
    extern const std::string_view eventsPerformanceLog;
    ScopedTimer timer{"    Test condition 'leader to zone'", eventsPerformanceLog};

    extern long long conditionsTotalTime;
    ScopedValueTimer valueTimer{conditionsTotalTime};
#endif

    if (thisptr->stackId == emptyId) {
        return false;
    }

    const CMidgardID& stackId{thisptr->condLeaderToZone->stackId};
    const CMidgardID& locationId{thisptr->condLeaderToZone->locationId};

    if (CMidgardIDApi::get().getType(&stackId) == IdType::Stack) {
        const CMidStack* stack{getStack(objectMap, &thisptr->stackId)};
        return stack && isStackInsideLocation(objectMap, &stackId, &locationId);
    }

    // Instead of checking every stack we check location tiles.
    // Condition is met if there is a tile with a stack owned by specified player
    // and it was created from a template.
    // This approach reduces number of checks from O(N) to a maximum 49 in case of 7x7 location.

    const auto& getObjectId{CMidgardPlanApi::get().getObjectId};
    const CMidgardPlan* plan{getMidgardPlan(objectMap)};

    const CMidLocation* location{getLocation(objectMap, &locationId)};
    const CMqPoint& pos{location->position};
    const int diameter{location->radius * 2 + 1};
    const int radius{diameter / 2};

    const int startX{pos.x - radius};
    const int startY{pos.y - radius};
    const int endX{pos.x + radius + 1};
    const int endY{pos.y + radius + 1};

    const IdType type{IdType::Stack};
    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            const CMqPoint position{x, y};
            const CMidgardID* objectId{getObjectId(plan, &position, &type)};
            if (!objectId) {
                continue;
            }

            const CMidStack* stack{getStack(objectMap, objectId)};
            if (!stack) {
                continue;
            }

            if (stack->ownerId == *playerId && stack->sourceTemplateId == stackId) {
                return true;
            }
        }
    }

    return false;
}

} // namespace hooks
