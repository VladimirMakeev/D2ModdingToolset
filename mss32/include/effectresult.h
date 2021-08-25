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

#ifndef EFFECTRESULT_H
#define EFFECTRESULT_H

#include "linkedlist.h"

namespace game {

struct IEffectResultVftable;
struct String;
struct IMidgardObjectMap;
struct IEventEffect;
struct CMidgardID;

/**
 * Event effect results interface.
 * Effect results are created depending on CMidEvEffect::category.
 */
struct IEffectResult
{
    IEffectResultVftable* vftable;
};

struct IEffectResultVftable
{
    using Destructor = void(__thiscall*)(IEffectResult* thisptr, char flags);
    Destructor destructor;

    /**
     * Applies event effect result.
     * Wraps event effect logic by creating one or more
     * IEventEffect objects and adding them to the effects list
     * for later processing by game.
     * @param[inout] str unused by the game. Assumption: was used for debugging.
     * @param[in] objectMap used to search scenario objects.
     * @param[inout] effects list of effects that will be executed by the game.
     * @param[in] playerId affected player id.
     * @param[in] triggererId id of stack that triggered event.
     * @param[in] playingStackId id of stack currently playing. Assumption: not used and empty.
     */
    using Apply = void(__thiscall*)(const IEffectResult* thisptr,
                                    String* str,
                                    IMidgardObjectMap* objectMap,
                                    LinkedList<IEventEffect*>* effects,
                                    const CMidgardID* playerId,
                                    const CMidgardID* triggererId,
                                    const CMidgardID* playingStackId);
    Apply apply;

    /**
     * Returns true to indicate that further events and effects processing should stop.
     * Only CEffectResultBattle is known to return true from this method.
     */
    using StopProcessing = bool(__thiscall*)(const IEffectResult* thisptr);
    StopProcessing stopProcessing;
};

static_assert(sizeof(IEffectResultVftable) == 3 * sizeof(void*),
              "IEffectResult vftable must have exactly 3 methods");

} // namespace game

#endif // EFFECTRESULT_H
