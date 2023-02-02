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
#include "logutils.h"
#include "midgardscenariomap.h"
#include "midserver.h"
#include "midserverlogic.h"
#include "originalfunctions.h"
#include "refreshinfo.h"
#include "settings.h"
#include "utils.h"
#include <fmt/format.h>
#include <process.h>

namespace hooks {

bool __fastcall midServerLogicSendObjectsChangesHooked(game::IMidMsgSender* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto serverLogic = castMidMsgSenderToMidServerLogic(thisptr);
    const auto scenarioMap = CMidServerLogicApi::get().getObjectMap(serverLogic);
    const auto& addedObjects = scenarioMap->addedObjects;
    const auto& changedObjects = scenarioMap->changedObjects;
    const auto& objectsToErase = scenarioMap->objectsToErase;

    auto logFileName = fmt::format("netMessages{:d}.log", _getpid());
    logDebug(
        logFileName,
        fmt::format("Sending scenario objects changes, added: {:d}, changed: {:d}, erased: {:d}",
                    addedObjects.length, changedObjects.length, objectsToErase.length));

    auto totalLength = addedObjects.length + changedObjects.length + objectsToErase.length;
    if (totalLength >= userSettings().debug.sendObjectsChangesTreshold) {
        for (auto obj : addedObjects) {
            logDebug(logFileName, fmt::format("Added\t{:s}\t{:s}", idToString(&obj),
                                                    getMidgardIdTypeDesc(&obj)));
        }
        for (auto obj : changedObjects) {
            logDebug(logFileName, fmt::format("Changed\t{:s}\t{:s}", idToString(&obj),
                                                    getMidgardIdTypeDesc(&obj)));
        }
        for (auto obj : objectsToErase) {
            logDebug(logFileName, fmt::format("Erased\t{:s}\t{:s}", idToString(&obj),
                                                    getMidgardIdTypeDesc(&obj)));
        }
    }

    return getOriginalFunctions().midServerLogicSendObjectsChanges(thisptr);
}

bool __fastcall midServerLogicSendRefreshInfoHooked(const game::CMidServerLogic* thisptr,
                                                    int /*%edx*/,
                                                    const game::Set<game::CMidgardID>* objectsList,
                                                    std::uint32_t playerNetId)
{
    using namespace game;

    const auto& refreshInfoApi = CRefreshInfoApi::get();

    const auto scenarioMap = CMidServerLogicApi::get().getObjectMap(thisptr);
    const auto limit = userSettings().engine.sendRefreshInfoObjectCountLimit;

    auto it = objectsList->begin();
    const auto end = objectsList->end();
    auto sendRefreshInfo = [&](bool& proceed) {
        CRefreshInfo refreshInfo{};
        refreshInfoApi.constructor2(&refreshInfo, &scenarioMap->scenarioFileId,
                                    thisptr->coreData->isExpansionContent);

        proceed = false;
        std::uint32_t count = 0;
        for (; it != end; ++it) {
            if (++count > limit) {
                proceed = true;
                break;
            }
            refreshInfoApi.addObject(&refreshInfo, scenarioMap, &(*it));
        }

        auto server = thisptr->coreData->server;
        bool result = CMidServerApi::get().sendNetMsg(server, &refreshInfo, playerNetId);

        refreshInfoApi.destructor(&refreshInfo);
        return result;
    };

    for (bool proceed = true; proceed;) {
        if (!sendRefreshInfo(proceed)) {
            return false;
        }
    }

    return true;
}

} // namespace hooks
