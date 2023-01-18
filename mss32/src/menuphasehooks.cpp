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

    auto transitionPtr = &thisptr->data->transitionNumber;
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
            logDebug("transitions.log", "current is 21");
            menuPhase.switchToMain(thisptr);
            break;
        case 0:
            logDebug("transitions.log", "current is 0");
            menuPhase.transitionFromMain(thisptr, next);
            break;
        case 22:
            logDebug("transitions.log", "current is 22");
            menuPhase.switchToSingle(thisptr);
            break;
        case 1:
            logDebug("transitions.log", "current is 1");
            menuPhase.transitionFromSingle(thisptr, next);
            break;
        case 2: {
            logDebug("transitions.log", "current is 2");
            if (next == 2) {
                // Show new fullscreen animation
                auto data = thisptr->data;
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
            auto data = thisptr->data;
            CMenuPhaseApi::Api::CreateMenuCallback tmp = createCustomLobbyMenu;
            CMenuPhaseApi::Api::CreateMenuCallback* callback = &tmp;
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
                // Create random scenario single menu window during fullscreen animation
                auto data = thisptr->data;
                CMenuPhaseApi::Api::CreateMenuCallback tmp = createMenuRandomScenarioSingle;
                CMenuPhaseApi::Api::CreateMenuCallback* callback = &tmp;
                logDebug("transitions.log", "Try to transition to 37");
                menuPhase.doTransition(thisptr, &data->transitionNumber, &data->interfManager,
                                       &data->currentMenu, &data->transitionAnimation, 37, nullptr,
                                       &callback);
            } else {
                logError("mssProxyError.log", "Invalid next transition from state 6");
                return;
            }
            break;
        }
        case 37:
            // CMenuRandomScenarioSingle state
            logDebug("transitions.log", "current is 37");
            break;
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
            logDebug("transitions.log", "current is 23");
            menuPhase.switchToProtocol(thisptr);
            break;
        case 25:
            logDebug("transitions.log", "current is 25");
            menuPhase.switchToMulti(thisptr);
            break;
        case 4:
            logDebug("transitions.log", "current is 4");
            menuPhase.transitionFromMulti(thisptr, next);
            break;
        case 24:
            logDebug("transitions.log", "current is 24");
            menuPhase.switchToHotseat(thisptr);
            break;
        case 9:
            logDebug("transitions.log", "current is 9");
            menuPhase.switchToNewSkirmishHotseat(thisptr);
            break;
        case 10:
            logDebug("transitions.log", "current is 10");
            menuPhase.switchToLoadSkirmishHotseat(thisptr);
            break;
        case 30:
            logDebug("transitions.log", "current is 30");
            menuPhase.transitionFromNewSkirmishHotseat(thisptr);
            break;
        case 14:
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
            logDebug("transitions.log", "current is 16");
            menuPhase.switchToWait(thisptr);
            break;
        }

        break;
    }
}

} // namespace hooks
