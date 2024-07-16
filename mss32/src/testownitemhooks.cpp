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

#include "testownitemhooks.h"
#include "eventconditions.h"
#include "fortification.h"
#include "gameutils.h"
#include "midevent.h"
#include "midgardobjectmap.h"
#include "miditem.h"
#include "midstack.h"
#include "testownitem.h"
#include "timer.h"
#include "utils.h"
#include <unordered_set>

namespace hooks {

bool __fastcall testOwnItemHooked(const game::CTestOwnItem* thisptr,
                                  int /*%edx*/,
                                  const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* playerId,
                                  const game::CMidgardID* eventId)
{
    using namespace game;

#ifdef D2_MEASURE_EVENTS_TIME
    extern const std::string_view eventsPerformanceLog;
    ScopedTimer timer{"    Test condition 'own item'", eventsPerformanceLog};

    extern long long conditionsTotalTime;
    ScopedValueTimer valueTimer{conditionsTotalTime};
#endif

    // Get all players to which event can be applied, by their races
    std::unordered_set<CMidgardID, CMidgardIDHash> affectedPlayers;
    auto getAffectedPlayer = [objectMap, eventId,
                              &affectedPlayers](const IMidScenarioObject* player) {
        if (CMidEventApi::get().affectsPlayer(objectMap, &player->id, eventId)) {
            affectedPlayers.insert(player->id);
        }
    };

    forEachScenarioObject(objectMap, IdType::Player, getAffectedPlayer);

    const CMidgardID* itemId{&thisptr->condOwnItem->itemId};

    IteratorPtr current{};
    objectMap->vftable->begin(objectMap, &current);

    IteratorPtr end{};
    objectMap->vftable->end(objectMap, &end);

    const auto& getType{CMidgardIDApi::get().getType};
    const auto& free{SmartPointerApi::get().createOrFree};

    while (!current.data->vftable->end(current.data, end.data)) {
        const CMidgardID* id{current.data->vftable->getObjectId(current.data)};
        const IdType idType{getType(id)};

        if (idType == IdType::Stack) {
            const CMidStack* stack{getStack(objectMap, id)};

            if (affectedPlayers.find(stack->ownerId) != affectedPlayers.cend()) {
                if (isInventoryContainsItem(objectMap, stack->inventory, *itemId)) {
                    free((SmartPointer*)&current, nullptr);
                    free((SmartPointer*)&end, nullptr);
                    return true;
                }
            }
        } else if (idType == IdType::Fortification) {
            const CFortification* fort{getFort(objectMap, id)};

            if (affectedPlayers.find(fort->ownerId) != affectedPlayers.cend()) {
                if (isInventoryContainsItem(objectMap, fort->inventory, *itemId)) {
                    free((SmartPointer*)&current, nullptr);
                    free((SmartPointer*)&end, nullptr);
                    return true;
                }
            }
        }

        current.data->vftable->advance(current.data);
    }

    free((SmartPointer*)&current, nullptr);
    free((SmartPointer*)&end, nullptr);
    return false;
}

} // namespace hooks
