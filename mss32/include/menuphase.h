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

#ifndef MENUPHASE_H
#define MENUPHASE_H

#include "catalogvalidate.h"
#include "difficultylevel.h"
#include "midgardid.h"
#include "mqnetsystem.h"
#include "racelist.h"
#include "smartptr.h"
#include <cstddef>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace game {

struct CMidgard;
struct CInterface;
struct CInterfManagerImpl;
struct ScenarioDataArrayWrapped;
struct IMqImage2;
struct CMenuBase;

struct CMenuPhaseData
{
    CMidgard* midgard;
    CInterface* currentMenu;
    SmartPtr<CInterfManagerImpl> interfManager;
    int transitionNumber;
    ScenarioDataArrayWrapped* scenarios;
    SmartPtr<IMqImage2> transitionAnimation;
    int maxPlayers;
    bool loadScenario;
    bool host;
    bool useGameSpy;
    char padding2;
    RaceCategoryList races;
    LRaceCategory race;
    LDifficultyLevel difficultyLevel;
    char* scenarioFilePath;
    CMidgardID scenarioFileId;
    CMidgardID campaignId;
    char* scenarioName;
    char* scenarioDescription;
    int suggestedLevel;
    // Assumption: flag indicating singleplayer game
    bool unknown8;
    char padding[3];
    HANDLE scenarioFileHandle;
    int unknown10;
};

assert_size(CMenuPhaseData, 116);
assert_offset(CMenuPhaseData, races, 40);
assert_offset(CMenuPhaseData, scenarioFileId, 84);

struct CMenuPhase
    : public IMqNetSystem
    , public ICatalogValidate
{
    CMenuPhaseData* data;
};

assert_size(CMenuPhase, 12);
assert_offset(CMenuPhase, CMenuPhase::IMqNetSystem::vftable, 0);
assert_offset(CMenuPhase, CMenuPhase::ICatalogValidate::vftable, 4);

namespace CMenuPhaseApi {

struct Api
{
    using Constructor = CMenuPhase*(__thiscall*)(CMenuPhase* thisptr, int a2, int a3);
    Constructor constructor;

    /** Sets menu transition index, implements menu screen transitions logic. */
    using SetTransition = void(__thiscall*)(CMenuPhase* thisptr, int transition);
    SetTransition setTransition;

    using CreateMenuCallback = CMenuBase*(__stdcall*)(CMenuPhase* menuPhase);

    /**
     * Performs transition between two menu screens.
     * @param[inout] transition pointer to store transition index.
     * @param[in] interfManager manager to hide and show menu screens.
     * @param[in] nextMenu used to store next menu pointer after transition to it.
     * @param[in] animation transition animation.
     * @param nextTransition index of next menu screen.
     * @param[in] animationName animation name to show during transition, optional.
     * @param[in] callback callbacks that creates new menu object.
     */
    using DoTransition = void(__stdcall*)(CMenuPhase* menuPhase,
                                          int* transition,
                                          SmartPtr<CInterfManagerImpl>* interfManager,
                                          CInterface** nextMenu,
                                          SmartPtr<IMqImage2>* animation,
                                          int nextTransition,
                                          const char* animationName,
                                          CreateMenuCallback** callback);
    DoTransition doTransition;

    using ShowFullScreenAnimation = void(__stdcall*)(CMenuPhase* menuPhase,
                                                     int* transition,
                                                     SmartPtr<CInterfManagerImpl>* interfManager,
                                                     CInterface** currentMenu,
                                                     int nextTransition,
                                                     const char* animationName);
    ShowFullScreenAnimation showFullScreenAnimation;

    using SwitchToMenu = void(__thiscall*)(CMenuPhase* thisptr);
    using ShowTransition = void(__thiscall*)(CMenuPhase* thisptr, int nextTransition);

    // 21
    SwitchToMenu switchToMain;
    // 0
    ShowTransition transitionFromMain;
    // 22
    SwitchToMenu switchToSingle;
    // 1
    ShowTransition transitionFromSingle;
    // 2
    ShowTransition transitionFromProto;
    // 3
    ShowTransition transitionFromHotseat;
    // 26
    SwitchToMenu switchToRaceCampaign;
    // 5
    ShowTransition transitionGodToLord;
    // 29
    SwitchToMenu switchToCustomCampaign;
    // 8
    ShowTransition transitionNewQuestToGod;
    // 27
    SwitchToMenu switchToNewSkirmish;
    // 6
    SwitchToMenu switchTo15Or28;
    // 28
    SwitchToMenu switchToRaceSkirmish;
    // 7 reuses 5
    // 31
    SwitchToMenu switchToLord;
    // 33
    SwitchToMenu switchToLoadSkirmish;
    // 34
    SwitchToMenu switchToLoadCampaign;
    // 32
    SwitchToMenu switchToLoadCustomCampaign;
    // 17
    SwitchToMenu switchIntroToMain;
    // 18
    ShowTransition transitionToCredits;
    // 19 reuses 17
    // 23
    SwitchToMenu switchToProtocol;
    // 25
    SwitchToMenu switchToMulti;
    // 4
    ShowTransition transitionFromMulti;
    // 24
    SwitchToMenu switchToHotseat;
    // 9
    SwitchToMenu switchToNewSkirmishHotseat;
    // 10
    SwitchToMenu switchToLoadSkirmishHotseat;
    // 30
    SwitchToMenu transitionFromNewSkirmishHotseat;
    // 14
    SwitchToMenu switchToHotseatLobby;
    // 11
    SwitchToMenu switchToSession;
    // 15
    SwitchToMenu switchToLobbyHostJoin;
    // 16
    SwitchToMenu switchToWait;

    using SetString = void(__thiscall*)(CMenuPhase* thisptr, const char* string);

    SetString setScenarioFilePath;

    using SetCampaignId = void(__thiscall*)(CMenuPhase* thisptr, const CMidgardID* campaignId);
    SetCampaignId setCampaignId;

    SetString setScenarioName;
    SetString setScenarioDescription;
};

Api& get();

IMqNetSystemVftable* vftable();

} // namespace CMenuPhaseApi

} // namespace game

#endif // MENUPHASE_H
