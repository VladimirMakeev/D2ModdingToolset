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

#ifndef MENURACE_H
#define MENURACE_H

#include "menubase.h"
#include "racelist.h"
#include "sortedracelist.h"

namespace game {

struct IMqImage2;

struct CMenuRaceData
{
    int unknown;
    SortedRaceList racesToSkip;
    char unknown2[24];
    int screenIndex;
    SmartPtr<IMqImage2> backgroundImage;
    char unknown4;
    bool scrollBack;
    char padding[2];
};

static_assert(sizeof(CMenuRaceData) == 72,
              "Size of CMenuRaceData structure must be exactly 72 bytes");

static_assert(offsetof(CMenuRaceData, backgroundImage) == 60,
              "CMenuRaceData::backgroundImage offset must be 60 bytes");

/**
 * Base class for race selection menus.
 * Represents DLG_CHOOSE_RACE from Interf.dlg.
 */
struct CMenuRace : public CMenuBase
{
    CMenuRaceData* menuRaceData;
};

static_assert(sizeof(CMenuRace) == 16, "Size of CMenuRace structure must be exactly 16 bytes");

namespace CMenuRaceApi {

struct Api
{
    /** Used for changing race selection screens, background images, transition animations. */
    using ScrollButtonCallback = void(__thiscall*)(CMenuRace* thisptr);
    ScrollButtonCallback buttonNextCallback;
    ScrollButtonCallback buttonPrevCallback;

    using UpdateButtons = void(__stdcall*)(CMenuRace* thisptr);
    UpdateButtons updateButtons;

    /** Updates description text based on specified race category. */
    using UpdateRaceDescription = void(__stdcall*)(CDialogInterf* dialogInterf,
                                                   const LRaceCategory* raceCategory);
    UpdateRaceDescription updateRaceDescription;

    /** Returns race category that corresponds to screen index of CMenuRace. */
    using GetRaceCategory = LRaceCategory*(__stdcall*)(LRaceCategory* raceCategory,
                                                       int screenIndex);
    GetRaceCategory getRaceCategory;

    /** Returns background image name for specified race category. */
    using GetRaceBgndImageName = const char*(__stdcall*)(const LRaceCategory* raceCategory);
    GetRaceBgndImageName getRaceBgndImageName;

    /**
     * Returns race screen transition animation name based on race category and scroll direction.
     */
    using GetTransitionAnimationName = const char*(__stdcall*)(const LRaceCategory* raceCategory,
                                                               bool scrollBack);
    GetTransitionAnimationName getTransitionAnimationName;

    /**
     * Fills list with races that are not present on selected scenario map
     * and will be skipped during race selection.
     */
    using SetRacesToSkip = void(__stdcall*)(SortedRaceList* racesToSkip,
                                            const RaceCategoryList* scenarioRaces);
    SetRacesToSkip setRacesToSkip;
};

Api& get();

} // namespace CMenuRaceApi

} // namespace game

#endif // MENURACE_H
