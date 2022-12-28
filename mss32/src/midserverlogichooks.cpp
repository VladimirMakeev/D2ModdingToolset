/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "midserverlogichooks.h"
#include "log.h"
#include "midgardscenariomap.h"
#include "midserverlogic.h"
#include "originalfunctions.h"
#include "settings.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

bool __fastcall midServerLogicSendObjectsChangesHooked(game::IMidMsgSender* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto serverLogic = castMidMsgSenderToMidServerLogic(thisptr);
    const auto scenarioMap = (CMidgardScenarioMap*)CMidServerLogicApi::get().getObjectMap(
        serverLogic);
    const auto& addedObjects = scenarioMap->addedObjects;
    const auto& changedObjects = scenarioMap->changedObjects;

    logDebug("serverLogicSendObjects.log",
             fmt::format("Sending scenario objects changes, added: {:d}, changed: {:d}",
                         addedObjects.length, changedObjects.length));
    if (addedObjects.length + changedObjects.length > userSettings().debug.sendObjectsChangesTreshold) {
        for (auto obj : addedObjects) {
            logDebug("serverLogicSendObjects.log",
                     fmt::format("Sending added scenario object {:s}", idToString(&obj)));
        }
        for (auto obj : changedObjects) {
            logDebug("serverLogicSendObjects.log",
                     fmt::format("Sending changed scenario object {:s}", idToString(&obj)));
        }
    }

    return getOriginalFunctions().midServerLogicSendObjectsChanges(thisptr);
}

} // namespace hooks
