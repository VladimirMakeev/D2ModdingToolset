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

#ifndef MENURACEHOOKS_H
#define MENURACEHOOKS_H

#include "racelist.h"

namespace game {
struct CMenuRace;
struct CDialogInterf;
struct LRaceCategory;
} // namespace game

namespace hooks {

void __fastcall menuRaceBtnNextCallbackHooked(game::CMenuRace* thisptr, int /*%edx*/);
void __fastcall menuRaceBtnPrevCallbackHooked(game::CMenuRace* thisptr, int /*%edx*/);

void __stdcall updateRaceDescriptionHooked(game::CDialogInterf* dialogInterf,
                                           const game::LRaceCategory* raceCategory);

game::LRaceCategory* __stdcall getRaceCategoryHooked(game::LRaceCategory* raceCategory,
                                                     int screenIndex);

const char* __stdcall getRaceBgndImageNameHooked(const game::LRaceCategory* raceCategory);

const char* __stdcall getTransitionAnimationNameHooked(const game::LRaceCategory* raceCategory,
                                                       bool scrollBack);

game::RaceCategoryList* __stdcall getPlayableRacesHooked(game::RaceCategoryList* racesList);

} // namespace hooks

#endif // MENURACEHOOKS_H
