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

#include "menuphasehooks.h"
#include "log.h"
#include "menucustomlobby.h"
#include "menuphase.h"
#include "menurandomscenariomulti.h"
#include "menurandomscenariosingle.h"
#include "midgard.h"
#include "originalfunctions.h"
#include "scenariotemplates.h"
#include <fmt/format.h>

namespace hooks {

game::CMenuPhase* __fastcall menuPhaseCtorHooked(game::CMenuPhase* thisptr,
                                                 int /*%edx*/,
                                                 int a2,
                                                 int a3)
{
    getOriginalFunctions().menuPhaseCtor(thisptr, a2, a3);

    loadScenarioTemplates();

    return thisptr;
}

void __fastcall menuPhaseDtorHooked(game::CMenuPhase* thisptr, int /*%edx*/, char flags)
{
    freeScenarioTemplates();

    getOriginalFunctions().menuPhaseDtor(thisptr, flags);
}

void __fastcall menuPhaseSetTransitionHooked(game::CMenuPhase* thisptr,
                                             int /*%edx*/,
                                             int transition)
{
    using namespace game;

    using CreateMenuCallback = CMenuPhaseApi::Api::CreateMenuCallback;

    CMenuPhaseData* data = thisptr->data;

    auto transitionPtr = &data->transitionNumber;
    int current = *transitionPtr;
    int next = transition;

    logDebug("transitions.log",
             fmt::format("Current state {:d}, transition to {:d}", current, next));

    auto& midgardApi = CMidgardApi::get();
    auto& menuPhase = CMenuPhaseApi::get();

    while (true) {
        switch (current) {
        default:
            break;
        case 12: {
            logDebug("transitions.log", "current is 12");
            auto midgard = midgardApi.instance();
            midgardApi.setClientsNetProxy(midgard, thisptr);
            *transitionPtr = 15;
            goto label_5;
        }
        case 13: {
            logDebug("transitions.log", "current is 13");
            *transitionPtr = next != 0 ? 15 : 27;
        label_5:
            next = 0;
            transitionPtr = &thisptr->data->transitionNumber;
            current = *transitionPtr;
            if (*transitionPtr > 36) {
                return;
            }

            continue;
        }
        case 20:
            logDebug("transitions.log", "current is 20");
            thisptr->data->transitionNumber = 21;
            [[fallthrough]];
        case 21:
            // Creates CMenuMain, sets transition to 0
            logDebug("transitions.log", "current is 21");
            menuPhase.switchToMain(thisptr);
            break;
        case 0:
            // CMenuMain state.
            // next 0: shows 'TRANS_MAIN2SINGLE' animation, sets transition to 22
            // next 1: shows 'TRANS_MAIN2PROTO' animation, sets transition to 23
            // next 2: shows 'intro' animation, sets transition to 17
            // next 3: shows 'credits' animation, sets transition to 18
            logDebug("transitions.log", "current is 0");
            menuPhase.transitionFromMain(thisptr, next);
            break;
        case 22:
            // CMenuMain -> CMenuSingle animation state.
            // Creates CMenuSingle, sets transition to 1
            logDebug("transitions.log", "current is 22");
            menuPhase.switchToSingle(thisptr);
            break;
        case 1:
            // CMenuSingle state.
            // next 0: shows 'TRANS_SINGLE2GOD' animation, sets transition to 26
            // next 1: shows 'TRANS_SINGLE2LOAD' animation, sets transition to 34
            // next 2: shows 'TRANS_SINGLE2NEW' animation, sets transition to 27
            // next 3: shows 'TRANS_SINGLE2LOAD' animation, sets transition to 33
            // next 4: shows 'TRANS_SINGLE2NEW' animation, sets transition to 29
            // next 5: shows 'TRANS_SINGLE2NEW' animation, sets transition to 32
            logDebug("transitions.log", "current is 1");
            menuPhase.transitionFromSingle(thisptr, next);
            break;
        case 2: {
            // CMenuProtocol state
            logDebug("transitions.log", "current is 2");
            if (next == 2) {
                // Show new fullscreen animation
                logDebug("transitions.log", "Try to transition to 35 while playing animation");
                menuPhase.showFullScreenAnimation(thisptr, &data->transitionNumber,
                                                  &data->interfManager, &data->currentMenu, 35,
                                                  "TRANS_PROTO2MULTI");
            } else {
                menuPhase.transitionFromProto(thisptr, next);
            }
            break;
        }
        case 35: {
            // Create custom lobby menu window during fullscreen animation
            CreateMenuCallback tmp = createCustomLobbyMenu;
            CreateMenuCallback* callback = &tmp;
            logDebug("transitions.log", "Try to transition to 36");
            menuPhase.doTransition(thisptr, &data->transitionNumber, &data->interfManager,
                                   &data->currentMenu, &data->transitionAnimation, 36, nullptr,
                                   &callback);
        }
        case 36: {
            // CMenuCustomLobby state
            logDebug("transitions.log", "current is 36");
            break;
        }
        case 3:
            // CMenuHotseat state.
            // next 0: shows 'TRANS_HOTSEAT2NEW' animation, sets transition to 9
            // next 1: shows 'TRANS_HOTSEAT2LOAD' animation, sets transition to 10
            logDebug("transitions.log", "current is 3");
            menuPhase.transitionFromHotseat(thisptr, next);
            break;
        case 26:
            logDebug("transitions.log", "current is 26");
            menuPhase.switchToRaceCampaign(thisptr);
            break;
        case 5:
            logDebug("transitions.log", "current is 5");
            menuPhase.transitionGodToLord(thisptr, next);
            break;
        case 29:
            logDebug("transitions.log", "current is 29");
            menuPhase.switchToCustomCampaign(thisptr);
            break;
        case 8:
            logDebug("transitions.log", "current is 8");
            menuPhase.transitionNewQuestToGod(thisptr, next);
            break;
        case 27:
            logDebug("transitions.log", "current is 27");
            menuPhase.switchToNewSkirmish(thisptr);
            break;
        case 6: {
            logDebug("transitions.log", "current is 6");
            if (next == 0) {
                logDebug("transitions.log", "switch to 15 or 28");
                menuPhase.switchTo15Or28(thisptr);
            } else if (next == 1) {
                menuPhase.showFullScreenAnimation(thisptr, &data->transitionNumber,
                                                  &data->interfManager, &data->currentMenu, 37,
                                                  "TRANS_NEWQUEST2RNDSINGLE");
            } else if (next == 2) {
                // CMenuNewSkirmishMulti -> CMenuRandomScenarioMulti
                menuPhase.showFullScreenAnimation(thisptr, &data->transitionNumber,
                                                  &data->interfManager, &data->currentMenu, 41,
                                                  "TRANS_HOST2RNDMULTI");
            } else {
                logError("mssProxyError.log", "Invalid next transition from state 6");
                return;
            }
            break;
        }
        case 41: {
            // CMenuRandomScenarioMulti animation state
            logDebug("transitions.log", "current is 41");

            // Create random scenario multi menu window during fullscreen animaation
            CreateMenuCallback tmp = createMenuRandomScenarioMulti;
            CreateMenuCallback* callback = &tmp;
            logDebug("transitions.log", "Try to transition to 42");
            menuPhase.doTransition(thisptr, &data->transitionNumber, &data->interfManager,
                                   &data->currentMenu, &data->transitionAnimation, 42, nullptr,
                                   &callback);
            break;
        }
        case 42: {
            // CMenuRandomScenarioMulti state
            logDebug("transitions.log", "current is 42, switch to 15");
            menuPhase.switchTo15Or28(thisptr);
            break;
        }
        case 37: {
            // CMenuRandomScenarioSingle animation state

            logDebug("transitions.log", "current is 37");
            // Create random scenario single menu window during fullscreen animation
            CreateMenuCallback tmp = createMenuRandomScenarioSingle;
            CreateMenuCallback* callback = &tmp;
            logDebug("transitions.log", "Try to transition to 38");
            menuPhase.doTransition(thisptr, &data->transitionNumber, &data->interfManager,
                                   &data->currentMenu, &data->transitionAnimation, 38, nullptr,
                                   &callback);
            break;
        }
        case 38: {
            // CMenuRandomScenarioSingle state
            logDebug("transitions.log", "current is 38");
            menuPhase.showFullScreenAnimation(thisptr, &data->transitionNumber,
                                              &data->interfManager, &data->currentMenu, 28,
                                              "TRANS_RNDSINGLE2GOD");
            break;
        }
        case 28:
            logDebug("transitions.log", "current is 28");
            menuPhase.switchToRaceSkirmish(thisptr);
            break;
        case 7:
            logDebug("transitions.log", "current is 7");
            menuPhase.transitionGodToLord(thisptr, next);
            break;
        case 31:
            logDebug("transitions.log", "current is 31");
            menuPhase.switchToLord(thisptr);
            break;
        case 33:
            // CMenuMulti -> CMenuLoad animation state.
            // For GameSpy creates CMenuLoadSkirmishGameSpy, sets transition to 15
            // If loading scenario, creates CMenuLoadSkirmishMulti, sets transition to 15
            // Otherwise, creates CMenuLoad, sets transition to 16
            logDebug("transitions.log", "current is 33");
            menuPhase.switchToLoadSkirmish(thisptr);
            break;
        case 34:
            logDebug("transitions.log", "current is 34");
            menuPhase.switchToLoadCampaign(thisptr);
            break;
        case 32:
            logDebug("transitions.log", "current is 32");
            menuPhase.switchToLoadCustomCampaign(thisptr);
            break;
        case 17:
            // Shows 'TRANS_INTRO2MAIN', sets transition to 20
            logDebug("transitions.log", "current is 17");
            menuPhase.switchIntroToMain(thisptr);
            break;
        case 18:
            logDebug("transitions.log", "current is 18");
            menuPhase.transitionToCredits(thisptr, next);
            break;
        case 19:
            logDebug("transitions.log", "current is 19");
            menuPhase.switchIntroToMain(thisptr);
            break;
        case 23:
            // CMenuMain -> CMenuProtocol animation state.
            // Creates CMenuProtocol, sets transition to 2
            logDebug("transitions.log", "current is 23");
            menuPhase.switchToProtocol(thisptr);
            break;
        case 25:
            // CMenuProtocol -> CMenuMulti animation state.
            // Creates CMenuMulti, sets transition to 4.
            logDebug("transitions.log", "current is 25");
            menuPhase.switchToMulti(thisptr);
            break;
        case 4:
            // CMenuMulti state.
            // next 0: shows 'TRANS_MULTI2HOST' animation, sets transition to 27
            // next 1: shows 'TRANS_MULTI2JOIN' animation, sets transition to 11
            // next 2: shows 'TRANS_MULTI2LOAD' animation, sets transition to 33
            logDebug("transitions.log", "current is 4");
            menuPhase.transitionFromMulti(thisptr, next);
            break;
        case 24:
            // CMenuProtocol -> CMenuHotseat animation state.
            // Creates CMenuHotseat, sets transition to 3.
            logDebug("transitions.log", "current is 24");
            menuPhase.switchToHotseat(thisptr);
            break;
        case 9:
            // CMenuHotseat -> CMenuNewSkirmishHotseat animation state.
            // Creates CMenuNewSkirmishHotseat, sets transition to 30
            logDebug("transitions.log", "current is 9");
            menuPhase.switchToNewSkirmishHotseat(thisptr);
            break;
        case 10:
            // CMenuHotseat -> CMenuLoadSkirmishHotseat animation state.
            // Creates CMenuLoadSkirmishHotseat, sets transition to 16
            logDebug("transitions.log", "current is 10");
            menuPhase.switchToLoadSkirmishHotseat(thisptr);
            break;
        case 30: {
            // CMenuNewSkirmishHotseat state.
            logDebug("transitions.log", "current is 30");

            if (next == 0) {
                // Shows 'TRANS_NEW2HSLOBBY' animation, sets transition to 14
                menuPhase.showFullScreenAnimation(thisptr, &data->transitionNumber,
                                                  &data->interfManager, &data->currentMenu, 14,
                                                  "TRANS_NEW2HSLOBBY");
            } else if (next == 1) {
                // Reuse animation when transitioning from CMenuNewSkirmishHotseat
                // to CMenuRandomScenarioSingle
                menuPhase.showFullScreenAnimation(thisptr, &data->transitionNumber,
                                                  &data->interfManager, &data->currentMenu, 39,
                                                  "TRANS_NEWQUEST2RNDSINGLE");
            } else {
                logError("mssProxyError.log", "Invalid next transition from state 30");
                return;
            }

            break;
        }
        case 39: {
            // CMenuNewSkirmishHotseat -> CMenuRandomScenarioSingle animation state.
            // Create random scenario single menu window during fullscreen animation.
            // Reuse CMenuRandomScenarioSingle for hotseat games
            CreateMenuCallback tmp = createMenuRandomScenarioSingle;
            CreateMenuCallback* callback = &tmp;
            logDebug("transitions.log", "current is 39, try to transition to 40");
            menuPhase.doTransition(thisptr, &data->transitionNumber, &data->interfManager,
                                   &data->currentMenu, &data->transitionAnimation, 40, nullptr,
                                   &callback);
            break;
        }
        case 40: {
            // CMenuRandomScenarioSingle state for hotseat game mode.
            // Creates CMenuHotseatLobby, sets transition to 14
            logDebug("transitions.log", "current is 40");
            menuPhase.showFullScreenAnimation(thisptr, &data->transitionNumber,
                                              &data->interfManager, &data->currentMenu, 14,
                                              "TRANS_RND2HSLOBBY");
            break;
        }
        case 14:
            // CMenuNewSkirmishHotseat -> CMenuHotseatLobby animation state.
            // Creates CMenuHotseatLobby, sets transition to 16
            logDebug("transitions.log", "current is 14");
            menuPhase.switchToHotseatLobby(thisptr);
            break;
        case 11:
            logDebug("transitions.log", "current is 11");
            menuPhase.switchToSession(thisptr);
            break;
        case 15:
            logDebug("transitions.log", "current is 15");
            menuPhase.switchToLobbyHostJoin(thisptr);
            break;
        case 16:
            // Creates CMidClient, deletes CMenuPhase
            logDebug("transitions.log", "current is 16");
            menuPhase.switchToWait(thisptr);
            break;
        }

        break;
    }
}

} // namespace hooks
