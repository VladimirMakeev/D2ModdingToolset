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

#ifndef MIDSERVERLOGICHOOKS_H
#define MIDSERVERLOGICHOOKS_H

#include "d2pair.h"
#include "d2set.h"
#include "idlist.h"
#include "mqpoint.h"

namespace game {
struct IMidMsgSender;
struct CMidServerLogic;
struct IMidgardObjectMap;
struct CMidEvent;
struct IEventEffect;
struct ITestCondition;
} // namespace game

namespace hooks {

bool __fastcall midServerLogicSendObjectsChangesHooked(game::IMidMsgSender* thisptr, int /*%edx*/);

bool __fastcall midServerLogicSendRefreshInfoHooked(const game::CMidServerLogic* thisptr,
                                                    int /*%edx*/,
                                                    const game::Set<game::CMidgardID>* objectsList,
                                                    std::uint32_t playerNetId);

bool __fastcall applyEventEffectsAndCheckMidEventTriggerersHooked(
    game::CMidServerLogic** thisptr,
    int /*%edx*/,
    game::List<game::IEventEffect*>* effectsList,
    const game::CMidgardID* triggererId,
    const game::CMidgardID* playingStackId);

bool __fastcall stackMoveHooked(game::CMidServerLogic** thisptr,
                                int /*%edx*/,
                                const game::CMidgardID* playerId,
                                game::List<game::Pair<game::CMqPoint, int>>* movementPath,
                                const game::CMidgardID* stackId,
                                const game::CMqPoint* startingPoint,
                                const game::CMqPoint* endPoint);

bool __stdcall filterAndProcessEventsNoPlayerHooked(game::IMidgardObjectMap* objectMap,
                                                    game::List<game::CMidEvent*>* eventObjectList,
                                                    game::List<game::IEventEffect*>* effectsList,
                                                    bool* stopProcessing,
                                                    game::IdList* executedEvents,
                                                    const game::CMidgardID* triggererStackId,
                                                    const game::CMidgardID* playingStackId);

bool __stdcall filterAndProcessEventsHooked(game::IMidgardObjectMap* objectMap,
                                            game::List<game::CMidEvent*>* eventObjectList,
                                            game::List<game::IEventEffect*>* effectsList,
                                            bool* stopProcessing,
                                            game::IdList* executedEvents,
                                            const game::CMidgardID* playerId,
                                            const game::CMidgardID* triggererStackId,
                                            const game::CMidgardID* playingStackId);

bool __stdcall checkEventConditionsHooked(const game::IMidgardObjectMap* objectMap,
                                          game::List<game::IEventEffect*>* effectsList,
                                          const game::CMidgardID* playerId,
                                          const game::CMidgardID* stackTriggererId,
                                          int samePlayer,
                                          const game::CMidgardID* eventId);

void __stdcall executeEventEffectsHooked(game::IMidgardObjectMap* objectMap,
                                         game::List<game::IEventEffect*>* effectsList,
                                         bool* stopProcessing,
                                         const game::CMidgardID* eventId,
                                         const game::CMidgardID* playerId,
                                         const game::CMidgardID* stackTriggererId,
                                         const game::CMidgardID* playingStackId);

bool __fastcall testFreqHooked(const game::ITestCondition* thisptr,
                               int /*%edx*/,
                               const game::IMidgardObjectMap* objectMap,
                               const game::CMidgardID* playerId,
                               const game::CMidgardID* eventId);

bool __fastcall testLocationHooked(const game::ITestCondition* thisptr,
                                   int /*%edx*/,
                                   const game::IMidgardObjectMap* objectMap,
                                   const game::CMidgardID* playerId,
                                   const game::CMidgardID* eventId);

bool __fastcall testEnterCityHooked(const game::ITestCondition* thisptr,
                                    int /*%edx*/,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* playerId,
                                    const game::CMidgardID* eventId);

bool __fastcall testLeaderToCityHooked(const game::ITestCondition* thisptr,
                                       int /*%edx*/,
                                       const game::IMidgardObjectMap* objectMap,
                                       const game::CMidgardID* playerId,
                                       const game::CMidgardID* eventId);

bool __fastcall testOwnCityHooked(const game::ITestCondition* thisptr,
                                  int /*%edx*/,
                                  const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* playerId,
                                  const game::CMidgardID* eventId);

bool __fastcall testOwnItemHooked(const game::ITestCondition* thisptr,
                                  int /*%edx*/,
                                  const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* playerId,
                                  const game::CMidgardID* eventId);

bool __fastcall testLeaderOwnItemHooked(const game::ITestCondition* thisptr,
                                        int /*%edx*/,
                                        const game::IMidgardObjectMap* objectMap,
                                        const game::CMidgardID* playerId,
                                        const game::CMidgardID* eventId);

bool __fastcall testDiplomacyHooked(const game::ITestCondition* thisptr,
                                    int /*%edx*/,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* playerId,
                                    const game::CMidgardID* eventId);

bool __fastcall testAllianceHooked(const game::ITestCondition* thisptr,
                                   int /*%edx*/,
                                   const game::IMidgardObjectMap* objectMap,
                                   const game::CMidgardID* playerId,
                                   const game::CMidgardID* eventId);

bool __fastcall testLootRuinHooked(const game::ITestCondition* thisptr,
                                   int /*%edx*/,
                                   const game::IMidgardObjectMap* objectMap,
                                   const game::CMidgardID* playerId,
                                   const game::CMidgardID* eventId);

bool __fastcall testTransformLandHooked(const game::ITestCondition* thisptr,
                                        int /*%edx*/,
                                        const game::IMidgardObjectMap* objectMap,
                                        const game::CMidgardID* playerId,
                                        const game::CMidgardID* eventId);

bool __fastcall testVisitSiteHooked(const game::ITestCondition* thisptr,
                                    int /*%edx*/,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* playerId,
                                    const game::CMidgardID* eventId);

bool __fastcall testItemToLocationHooked(const game::ITestCondition* thisptr,
                                         int /*%edx*/,
                                         const game::IMidgardObjectMap* objectMap,
                                         const game::CMidgardID* playerId,
                                         const game::CMidgardID* eventId);

bool __fastcall testStackExistsHooked(const game::ITestCondition* thisptr,
                                      int /*%edx*/,
                                      const game::IMidgardObjectMap* objectMap,
                                      const game::CMidgardID* playerId,
                                      const game::CMidgardID* eventId);

bool __fastcall testVarInRangeHooked(const game::ITestCondition* thisptr,
                                     int /*%edx*/,
                                     const game::IMidgardObjectMap* objectMap,
                                     const game::CMidgardID* playerId,
                                     const game::CMidgardID* eventId);

} // namespace hooks

#endif // MIDSERVERLOGICHOOKS_H
