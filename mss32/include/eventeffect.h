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

#ifndef EVENTEFFECT_H
#define EVENTEFFECT_H

#include "idvector.h"

namespace game {

struct IEventEffectVftable;
struct IMidgardObjectMap;
struct IMidMsgSender;

struct IEventEffect
{
    IEventEffectVftable* vftable;
};

struct IEventEffectVftable
{
    using Destructor = void(__thiscall*)(IEventEffect* thisptr, char flags);
    Destructor destructor;

    /**
     * Applies game internal event effect.
     * Sends network messages using msgSender.
     * @returns true if effect applied successfully.
     */
    using Apply = bool(__thiscall*)(const IEventEffect* thisptr,
                                    IMidgardObjectMap* objectMap,
                                    IMidMsgSender* msgSender,
                                    IdVector* triggerers);
    Apply apply;

    using Method2 = bool(__thiscall*)(IEventEffect* thisptr, int a2);
    Method2 method2;

    using Method3 = bool(__thiscall*)(IEventEffect* thisptr);
    Method3 method3;

    /**
     * Returns true to indicate that further events and effects processing should stop.
     * Only CEffectBattle is known to return true from this method.
     */
    using StopProcessing = bool(__thiscall*)(const IEventEffect* thisptr);
    StopProcessing stopProcessing;
};

static_assert(sizeof(IEventEffectVftable) == 5 * sizeof(void*),
              "IEventEffect vftable must have exactly 5 methods");

} // namespace game

#endif // EVENTEFFECT_H
