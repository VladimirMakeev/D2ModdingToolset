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

namespace game {

struct CMidgardID;
struct IMidgardObjectMap;
struct CMidPlayer;
struct LRaceCategory;

/**
 * Returns player id depending on RAD_CASTER radio button selection in DLG_EFFECT_CASTMAP dialog.
 */
using RadioButtonIndexToPlayerId = CMidgardID*(__stdcall*)(CMidgardID* playerId,
                                                           IMidgardObjectMap* objectMap,
                                                           int index);

/** Searches for player by race category, returns nullptr if player could not be found. */
using FindPlayerByRaceCategory = CMidPlayer*(__stdcall*)(const LRaceCategory* raceCategory,
                                                         IMidgardObjectMap* objectMap);

/** Scenario Editor functions that can be hooked. */
struct EditorFunctions
{
    RadioButtonIndexToPlayerId radioButtonIndexToPlayerId;
    FindPlayerByRaceCategory findPlayerByRaceCategory;
};

extern EditorFunctions editorFunctions;

} // namespace game

#endif // EDITOR_H
