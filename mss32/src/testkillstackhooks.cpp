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

#include "testkillstackhooks.h"
#include "eventconditions.h"
#include "gameutils.h"
#include "midevent.h"
#include "midplayer.h"
#include "midstackdestroyed.h"
#include "testkillstack.h"
#include "timer.h"

namespace hooks {

bool __fastcall testKillStackHooked(const game::CTestKillStack* thisptr,
                                    int /*%edx*/,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* playerId,
                                    const game::CMidgardID* eventId)
{
    using namespace game;

#ifdef D2_MEASURE_EVENTS_TIME
    extern const std::string_view eventsPerformanceLog;
    ScopedTimer timer{"    Test condition 'kill stack'", eventsPerformanceLog};

    extern long long conditionsTotalTime;
    ScopedValueTimer valueTimer{conditionsTotalTime};
#endif

    const CMidStackDestroyed* stackDestroyed{getStackDestroyed(objectMap)};
    const CMidgardID& killStackId{thisptr->condKillStack->stackId};
    const CMidPlayer* neutrals{};

    const auto& affectsPlayer{CMidEventApi::get().affectsPlayer};

    for (const auto& entry : stackDestroyed->destroyedStacks) {
        if (entry.stackId == killStackId || entry.stackSrcTemplateId == killStackId) {
            CMidgardID killerId{entry.killerId};
            if (killerId == emptyId) {
                if (!neutrals) {
                    // Search for neutrals only when necessary
                    neutrals = getNeutralPlayer(objectMap);
                }

                killerId = neutrals->id;
            }

            if (affectsPlayer(objectMap, &killerId, eventId)) {
                return true;
            }
        }
    }

    return false;
}

} // namespace hooks
