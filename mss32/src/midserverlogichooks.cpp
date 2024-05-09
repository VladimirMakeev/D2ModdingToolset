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
#include "dynamiccast.h"
#include "exchangeresourcesmsg.h"
#include "gameutils.h"
#include "idset.h"
#include "log.h"
#include "logutils.h"
#include "midgardscenariomap.h"
#include "midplayer.h"
#include "midserver.h"
#include "midserverlogic.h"
#include "midsiteresourcemarket.h"
#include "midstack.h"
#include "netmsgcallbacks.h"
#include "netmsgmapentryexchangeresourcesmsg.h"
#include "netplayerinfo.h"
#include "originalfunctions.h"
#include "racetype.h"
#include "refreshinfo.h"
#include "settings.h"
#include "timer.h"
#include "unitstovalidate.h"
#include "unitutils.h"
#include "utils.h"
#include <chrono>
#include <fmt/format.h>
#include <process.h>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace hooks {

extern const std::string_view eventsPerformanceLog{"eventsPerformance.log"};

long long conditionsTotalTime = 0;
long long conditionsSystemTime = 0;
long long effectsTime = 0;

static bool __fastcall exchangeResourcesMsgHandler(game::CMidServerLogic* thisptr,
                                                   int /*%edx*/,
                                                   const CExchangeResourcesMsg* netMessage,
                                                   std::uint32_t idFrom)
{
    using namespace game;

    const NetPlayerInfo* playerInfo{CMidServerLogicApi::get().getPlayerInfo(thisptr, idFrom)};
    if (!playerInfo) {
        return false;
    }

    if (!CMidServerLogicApi::get().isCurrentPlayer(thisptr, &playerInfo->playerId)) {
        return true;
    }

    auto objectMap{thisptr->coreData->objectMap};
    if (!exchangeResources(objectMap, netMessage->siteId, netMessage->visitorStackId,
                           netMessage->playerCurrency, netMessage->siteCurrency,
                           netMessage->amount)) {
        return false;
    }

    thisptr->IMidMsgSender::vftable->sendObjectsChanges(thisptr);
    return true;
}

void addValidatedUnitsToChangedObjects(game::CMidgardScenarioMap* scenarioMap)
{
    using namespace game;

    const auto& idSetApi = IdSetApi::get();

    auto& changedObjects = scenarioMap->changedObjects;

    auto& unitsToValidate = getUnitsToValidate();
    for (const auto& unitId : unitsToValidate) {
        auto unit = (CMidUnit*)scenarioMap->vftable->findScenarioObjectById(scenarioMap, &unitId);
        if (unit && validateUnit(unit)) {
            Pair<IdSetIterator, bool> tmp;
            idSetApi.insert(&changedObjects, &tmp, &unitId);
        }
    }
    unitsToValidate.clear();
}

void logObjectsToSend(const game::CMidgardScenarioMap* scenarioMap)
{
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
}

bool __fastcall midServerLogicSendObjectsChangesHooked(game::IMidMsgSender* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto serverLogic = castMidMsgSenderToMidServerLogic(thisptr);
    auto scenarioMap = CMidServerLogicApi::get().getObjectMap(serverLogic);

    if (userSettings().modifiers.validateUnitsOnGroupChanged) {
        addValidatedUnitsToChangedObjects(scenarioMap);
    }

    if (userSettings().debugMode && userSettings().debug.sendObjectsChangesTreshold) {
        logObjectsToSend(scenarioMap);
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

bool __fastcall applyEventEffectsAndCheckMidEventTriggerersHooked(
    game::CMidServerLogic** thisptr,
    int /*%edx*/,
    game::List<game::IEventEffect*>* effectsList,
    const game::CMidgardID* triggererId,
    const game::CMidgardID* playingStackId)
{
#if 0
    const ScopedTimer timer{"Apply event effects and check triggerers", eventsPerformanceLog};
#endif

    return getOriginalFunctions().applyEventEffectsAndCheckMidEventTriggerers(thisptr, effectsList,
                                                                              triggererId,
                                                                              playingStackId);
}

bool __fastcall stackMoveHooked(game::CMidServerLogic** thisptr,
                                int /*%edx*/,
                                const game::CMidgardID* playerId,
                                game::List<game::Pair<game::CMqPoint, int>>* movementPath,
                                const game::CMidgardID* stackId,
                                const game::CMqPoint* startingPoint,
                                const game::CMqPoint* endPoint)
{
    using namespace game;

#if 0
    char message[256];
    const auto result{fmt::format_to_n(message, sizeof(message) - 1u,
                                       "Stack move from ({:d}, {:d}) to ({:d}, {:d})",
                                       startingPoint->x, startingPoint->y, endPoint->x,
                                       endPoint->y)};
    message[result.size] = 0;

    const ScopedTimer timer{std::string_view{message, result.size}, eventsPerformanceLog};
#endif

    return getOriginalFunctions().stackMove(thisptr, playerId, movementPath, stackId, startingPoint,
                                            endPoint);
}

bool __stdcall filterAndProcessEventsNoPlayerHooked(game::IMidgardObjectMap* objectMap,
                                                    game::List<game::CMidEvent*>* eventObjectList,
                                                    game::List<game::IEventEffect*>* effectsList,
                                                    bool* stopProcessing,
                                                    game::IdList* executedEvents,
                                                    const game::CMidgardID* triggererStackId,
                                                    const game::CMidgardID* playingStackId)
{
    using namespace game;

#if 0
    const ScopedTimer timer{"Filter and process events (no player)", eventsPerformanceLog};
#endif

    return CMidServerLogicApi::get().filterAndProcessEvents(objectMap, eventObjectList, effectsList,
                                                            stopProcessing, executedEvents,
                                                            &emptyId, triggererStackId,
                                                            playingStackId);
}

bool __stdcall filterAndProcessEventsHooked(game::IMidgardObjectMap* objectMap,
                                            game::List<game::CMidEvent*>* eventObjectList,
                                            game::List<game::IEventEffect*>* effectsList,
                                            bool* stopProcessing,
                                            game::IdList* executedEvents,
                                            const game::CMidgardID* playerId,
                                            const game::CMidgardID* triggererStackId,
                                            const game::CMidgardID* playingStackId)
{
    using namespace game;

#if 0
    const ScopedTimer timer{"Filter and process events", eventsPerformanceLog};
#endif

    if (*playerId != emptyId && gameFunctions().ignorePlayerEvents(playerId, objectMap)) {
        return false;
    }

    CMidgardID triggererId{*triggererStackId};
    const CMidStack* triggererStack{};

    if (triggererId != emptyId) {
        triggererStack = getStack(objectMap, &triggererId);
        if (!triggererStack && triggererId != emptyId) {
            triggererId = emptyId;
        }
    }

    // Use unordered set for fast lookup of executed events
    std::unordered_set<CMidgardID, CMidgardIDHash> executedEventsSet(executedEvents->length);
    for (const CMidgardID& id : *executedEvents) {
        executedEventsSet.insert(id);
    }

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    // Cache players so we won't waste time searching them every time
    std::unordered_map<RaceId, CMidgardID /* player id */> racePlayerMap;

    auto cachePlayer = [triggererStack, &racePlayerMap, &rtti, &dynamicCast](const auto* obj) {
        auto* player = (const CMidPlayer*)dynamicCast(obj, 0, rtti.IMidScenarioObjectType,
                                                      rtti.CMidPlayerType, 0);

        if (!triggererStack || player->id == triggererStack->ownerId) {
            const RaceId raceId{player->raceType->data->raceType.id};
            racePlayerMap[raceId] = player->id;
        }
    };

    forEachScenarioObject(objectMap, IdType::Player, cachePlayer);

    const auto& raceSetFind{RaceSetApi::get().find};
    const auto& checkAndExecuteEvent{CMidServerLogicApi::get().checkAndExecuteEvent};

    for (const CMidEvent* evt : *eventObjectList) {
        if (!evt->enabled) {
            // Event disabled, don't waste time checking anything
            continue;
        }

        if (executedEventsSet.find(evt->id) != executedEventsSet.end()) {
            // Event already executed, skip
            continue;
        }

        const auto end{evt->racesCanTrigger.end()};

        bool eventExecuted = false;
        LRaceCategory raceCategory{};
        SetIterator<LRaceCategory> it{};
        for (const auto& [raceId, playerCanTriggerId] : racePlayerMap) {
            // Only category id is checked during search. Don't bother setting other fields
            raceCategory.id = raceId;
            raceSetFind(&evt->racesCanTrigger, &it, &raceCategory);

            if (it == end) {
                // Race can't trigger
                continue;
            }

#ifdef D2_MEASURE_EVENTS_TIME
            conditionsTotalTime = 0;
            conditionsSystemTime = 0;
            effectsTime = 0;

            char message[256];
            const auto result{fmt::format_to_n(message, sizeof(message) - 1u, "  Event '{:s}'",
                                               evt->name.string ? evt->name.string : "?")};
            message[result.size] = 0;

            {
                const ScopedTimer eventTimer{std::string_view{message, result.size},
                                             eventsPerformanceLog};
#endif

                const bool samePlayer = *playerId == playerCanTriggerId;
                // Player can trigger, check event conditions and execute effects
                if (checkAndExecuteEvent(objectMap, effectsList, stopProcessing, &evt->id,
                                         &playerCanTriggerId, &triggererId, playingStackId,
                                         samePlayer)) {
                    eventExecuted = true;
                }

#ifdef D2_MEASURE_EVENTS_TIME
            }

            const auto overhead{conditionsSystemTime - conditionsTotalTime};
            logDebug("eventsPerformance.log",
                     fmt::format("{:s} conditions time {:d} us, "
                                 "conditions system time {:d} us (overhead {:d} us), "
                                 "effects time {:d} us",
                                 message, conditionsTotalTime, conditionsSystemTime, overhead,
                                 effectsTime));
#endif
        }

        if (eventExecuted) {
            IdListApi::get().pushBack(executedEvents, &evt->id);
            return true;
        }
    }

    return false;
}

bool __stdcall checkEventConditionsHooked(const game::IMidgardObjectMap* objectMap,
                                          game::List<game::IEventEffect*>* effectsList,
                                          const game::CMidgardID* playerId,
                                          const game::CMidgardID* stackTriggererId,
                                          int samePlayer,
                                          const game::CMidgardID* eventId)
{
    using namespace game;

#ifdef D2_MEASURE_EVENTS_TIME
    const ScopedValueTimer timer{conditionsSystemTime};
#endif

    return getOriginalFunctions().checkEventConditions(objectMap, effectsList, playerId,
                                                       stackTriggererId, samePlayer, eventId);
}

void __stdcall executeEventEffectsHooked(game::IMidgardObjectMap* objectMap,
                                         game::List<game::IEventEffect*>* effectsList,
                                         bool* stopProcessing,
                                         const game::CMidgardID* eventId,
                                         const game::CMidgardID* playerId,
                                         const game::CMidgardID* stackTriggererId,
                                         const game::CMidgardID* playingStackId)
{
    using namespace game;

#ifdef D2_MEASURE_EVENTS_TIME
    const ScopedValueTimer timer{effectsTime};
#endif

    getOriginalFunctions().executeEventEffects(objectMap, effectsList, stopProcessing, eventId,
                                               playerId, stackTriggererId, playingStackId);
}

static bool doTestHooked(game::ITestConditionVftable::Test testFunc,
                         const char* name,
                         const game::ITestCondition* thisptr,
                         const game::IMidgardObjectMap* objectMap,
                         const game::CMidgardID* playerId,
                         const game::CMidgardID* eventId)
{
    using namespace game;

#ifdef D2_MEASURE_EVENTS_TIME
    char message[256];
    const auto result{
        fmt::format_to_n(message, sizeof(message) - 1u, "    Test condition '{:s}'", name)};
    message[result.size] = 0;

    const ScopedTimer conditionTimer{std::string_view{message, result.size}, eventsPerformanceLog};
    const ScopedValueTimer timer{conditionsTotalTime};
#endif

    return testFunc(thisptr, objectMap, playerId, eventId);
}

bool __fastcall testFreqHooked(const game::ITestCondition* thisptr,
                               int /*%edx*/,
                               const game::IMidgardObjectMap* objectMap,
                               const game::CMidgardID* playerId,
                               const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testFrequency, "frequency", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testLocationHooked(const game::ITestCondition* thisptr,
                                   int /*%edx*/,
                                   const game::IMidgardObjectMap* objectMap,
                                   const game::CMidgardID* playerId,
                                   const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testLocation, "location", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testEnterCityHooked(const game::ITestCondition* thisptr,
                                    int /*%edx*/,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* playerId,
                                    const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testEnterCity, "enter city", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testLeaderToCityHooked(const game::ITestCondition* thisptr,
                                       int /*%edx*/,
                                       const game::IMidgardObjectMap* objectMap,
                                       const game::CMidgardID* playerId,
                                       const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testLeaderToCity, "leader to city", thisptr,
                        objectMap, playerId, eventId);
}

bool __fastcall testOwnCityHooked(const game::ITestCondition* thisptr,
                                  int /*%edx*/,
                                  const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* playerId,
                                  const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testOwnCity, "own city", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testDiplomacyHooked(const game::ITestCondition* thisptr,
                                    int /*%edx*/,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* playerId,
                                    const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testDiplomacy, "diplomacy", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testAllianceHooked(const game::ITestCondition* thisptr,
                                   int /*%edx*/,
                                   const game::IMidgardObjectMap* objectMap,
                                   const game::CMidgardID* playerId,
                                   const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testAlliance, "alliance", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testLootRuinHooked(const game::ITestCondition* thisptr,
                                   int /*%edx*/,
                                   const game::IMidgardObjectMap* objectMap,
                                   const game::CMidgardID* playerId,
                                   const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testLootRuin, "loot ruin", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testTransformLandHooked(const game::ITestCondition* thisptr,
                                        int /*%edx*/,
                                        const game::IMidgardObjectMap* objectMap,
                                        const game::CMidgardID* playerId,
                                        const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testTransformLand, "transform land", thisptr,
                        objectMap, playerId, eventId);
}

bool __fastcall testVisitSiteHooked(const game::ITestCondition* thisptr,
                                    int /*%edx*/,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* playerId,
                                    const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testVisitSite, "visit site", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testItemToLocationHooked(const game::ITestCondition* thisptr,
                                         int /*%edx*/,
                                         const game::IMidgardObjectMap* objectMap,
                                         const game::CMidgardID* playerId,
                                         const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testItemToLocation, "item to location", thisptr,
                        objectMap, playerId, eventId);
}

bool __fastcall testVarInRangeHooked(const game::ITestCondition* thisptr,
                                     int /*%edx*/,
                                     const game::IMidgardObjectMap* objectMap,
                                     const game::CMidgardID* playerId,
                                     const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testVarInRange, "var in range", thisptr, objectMap,
                        playerId, eventId);
}

game::CMidServerLogic* __fastcall midServerLogicCtorHooked(game::CMidServerLogic* thisptr,
                                                           int /*%edx*/,
                                                           game::CMidServer* server,
                                                           bool multiplayerGame,
                                                           bool hotseatGame,
                                                           int a5,
                                                           int gameVersion)
{
    using namespace game;

    getOriginalFunctions().midServerLogicCtor(thisptr, server, multiplayerGame, hotseatGame, a5,
                                              gameVersion);

    auto netMsgEntryData{thisptr->coreData->netMsgEntryData};

    auto callback = (CNetMsgMapEntry_member::Callback)exchangeResourcesMsgHandler;
    auto entry{createNetMsgMapEntryExchangeResourcesMsg(thisptr, callback)};

    NetMsgApi::get().addEntry(netMsgEntryData, (CNetMsgMapEntry*)entry);
    return thisptr;
}

} // namespace hooks
