/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef CUSTOMNOBLEACTIONHOOKS_H
#define CUSTOMNOBLEACTIONHOOKS_H

#include "d2set.h"
#include "nobleactioncat.h"

namespace game {
struct INobleActionResult;
struct IMidgardObjectMap;
struct CMidgardID;
struct String;
struct CCmdNobleResultMsg;
struct CPhaseGame;
struct CMidPlayer;
} // namespace game

namespace hooks {

game::INobleActionResult* __stdcall createNobleActionResultHooked(
    game::IMidgardObjectMap* objectMap,
    const game::LNobleActionCat* actionCategory,
    const game::CMidgardID* targetObjectId,
    const game::CMidgardID* id);

bool __stdcall getSiteNobleActionsHooked(const game::IMidgardObjectMap* objectMap,
                                         const game::CMidgardID* playerId,
                                         const game::CMidgardID* objectId,
                                         game::Set<game::LNobleActionCat>* nobleActions);

bool __stdcall getPossibleNobleActionsHooked(const game::IMidgardObjectMap* objectMap,
                                             const game::CMidgardID* playerId,
                                             const game::CMidgardID* objectId,
                                             game::Set<game::LNobleActionCat>* nobleActions);

game::String* __stdcall getNobleActionResultDescriptionHooked(
    game::String* description,
    const game::LNobleActionCat nobleActionCat,
    const game::CCmdNobleResultMsg* nobleResultMsg,
    const game::CPhaseGame* phaseGame,
    const game::CMidPlayer* player);

} // namespace hooks

#endif // CUSTOMNOBLEACTIONHOOKS_H
