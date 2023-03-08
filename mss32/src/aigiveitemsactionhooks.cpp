/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#include "aigiveitemsactionhooks.h"
#include "aigiveitemsaction.h"
#include "aimovestackaction.h"
#include "aistackgeneric.h"
#include "dynamiccast.h"
#include "idset.h"
#include "midgardobjectmap.h"
#include "midserverlogic.h"
#include "midstack.h"

namespace hooks {

void __fastcall aiGiveItemsActionExecuteHooked(game::CAiGiveItemsAction* thisptr,
                                               int /*%edx*/,
                                               game::IMidgardObjectMap* objectMap,
                                               game::CMidServerLogicData* serverLogicData,
                                               game::IAiTacticInternal* tacticInternal)
{
    using namespace game;

    const auto& idSetApi = IdSetApi::get();
    const auto& serverLogicApi = CMidServerLogicApi::get();
    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    CAiMoveStackActionApi::vftable().action->execute(thisptr, objectMap, serverLogicData,
                                                     tacticInternal);

    // The stack can trigger battle/events during move action that might cause the stack to get
    // destroyed, causing AI logic to crash
    auto action = (IAiTacticAction*)thisptr;
    auto stackId = action->vftable->getStackId(action);
    auto stackObj = objectMap->vftable->findScenarioObjectById(objectMap, stackId);
    if (!stackObj) {
        return;
    }

    auto aiStack = thisptr->stack;
    auto targetPosition = action->vftable->getTargetPosition(action);
    if (aiStack->vftable->getDistance2(aiStack, targetPosition) != 1) {
        return;
    }

    IdSet itemIds{};
    idSetApi.constructor(&itemIds);

    auto stack = static_cast<const CMidStack*>(
        dynamicCast(stackObj, 0, rtti.IMidScenarioObjectType, rtti.CMidStackType, 0));

    const auto& items = stack->inventory.items;
    const auto count = items.end - items.bgn;
    for (const game::CMidgardID* it = items.bgn; it != items.end; it++) {
        Pair<IdSetIterator, bool> tmp{};
        idSetApi.insert(&itemIds, &tmp, it);
    }

    serverLogicApi.stackExchangeItem(serverLogicData, &stack->ownerId, stackId,
                                     action->vftable->getTargetId(action), &itemIds);

    idSetApi.destructor(&itemIds);
}

} // namespace hooks
