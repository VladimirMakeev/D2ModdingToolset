/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef EDITOR_H
#define EDITOR_H

#include "intvector.h"

namespace game {

struct CMidgardID;
struct IMidgardObjectMap;
struct CMidPlayer;
struct LRaceCategory;
struct LSubRaceCategory;
struct TRaceType;
struct CCapital;
struct CVisitorAddPlayer;
struct String;
struct CMqPoint;
struct CMidgardMap;
struct LSiteCategory;
struct IMqImage2;
struct CMidSite;
struct CTextBoxInterf;

/**
 * Returns player id depending on RAD_CASTER radio button selection in DLG_EFFECT_CASTMAP dialog.
 */
using RadioButtonIndexToPlayerId = CMidgardID*(__stdcall*)(CMidgardID* playerId,
                                                           IMidgardObjectMap* objectMap,
                                                           int index);

/** Searches for player by race category, returns nullptr if player could not be found. */
using FindPlayerByRaceCategory = CMidPlayer*(__stdcall*)(const LRaceCategory* raceCategory,
                                                         IMidgardObjectMap* objectMap);

/** Returns true if tiles are suitable for site or ruin. */
using CanPlace = bool(__stdcall*)(const CMqPoint* position,
                                  const CMidgardMap* map,
                                  const IMidgardObjectMap* objectMap);

/** Returns number of CMidStack objects on map. */
using CountStacksOnMap = int(__stdcall*)(IMidgardObjectMap* objectMap);

/** Returns sub race category that corresponds to specified race category. */
using GetSubRaceByRace = const LSubRaceCategory*(__stdcall*)(const LRaceCategory* raceCategory);

using IsRaceCategoryPlayable = bool(__stdcall*)(const LRaceCategory* raceCategory);

/** Changes terrain of tiles under the specified capital city. */
using ChangeCapitalTerrain = bool(__stdcall*)(const TRaceType* raceType,
                                              const CCapital* capital,
                                              IMidgardObjectMap* objectMap,
                                              CVisitorAddPlayer* visitor);

/**
 * Returns object name and position description string by id.
 * Used to describe objects used by event effects.
 */
using GetObjectNamePos = String*(__stdcall*)(String* description,
                                             const IMidgardObjectMap* objectMap,
                                             const CMidgardID* objectId);

/**
 * Returns indices (G000SI0000<suffix><index>) of site images in .ff files
 * for specified site category.
 */
using GetSiteImageIndices = void(__stdcall*)(IntVector* indices,
                                             const LSiteCategory* site,
                                             int animatedIso);

using GetSiteImage = IMqImage2*(__stdcall*)(const LSiteCategory* site,
                                            int imageIndex,
                                            bool animatedIso);

using GetSiteAtPosition = const CMidSite*(__stdcall*)(const CMqPoint* mapPosition,
                                                      const IMidgardObjectMap* objectMap);

using ShowOrHideSiteOnStrategicMap = void(__stdcall*)(const CMidSite* site,
                                                      const IMidgardObjectMap* objectMap,
                                                      const CMidgardID* playerId,
                                                      int a4);

/** Scenario Editor functions that can be hooked. */
struct EditorFunctions
{
    RadioButtonIndexToPlayerId radioButtonIndexToPlayerId;
    FindPlayerByRaceCategory findPlayerByRaceCategory;
    CanPlace canPlaceSite;
    CanPlace canPlaceRuin;
    CountStacksOnMap countStacksOnMap;
    GetSubRaceByRace getSubRaceByRace;
    IsRaceCategoryPlayable isRaceCategoryPlayable;
    ChangeCapitalTerrain changeCapitalTerrain;
    GetObjectNamePos getObjectNamePos;
    GetSiteImageIndices getSiteImageIndices;
    GetSiteImage getSiteImage;
    GetSiteAtPosition getSiteAtPosition;
    ShowOrHideSiteOnStrategicMap showOrHideSiteOnStrategicMap;
};

extern EditorFunctions editorFunctions;

} // namespace game

#endif // EDITOR_H
