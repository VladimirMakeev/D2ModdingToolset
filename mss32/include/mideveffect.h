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

#ifndef MIDEVEFFECT_H
#define MIDEVEFFECT_H

#include "eventeffectcat.h"
#include "midgardid.h"

namespace game {

struct CMidEvEffectVftable;
struct IMidgardObjectMap;
struct IMidgardStream;
struct IMidgardStreamEnv;

/** Base class for all event effects. */
struct CMidEvEffect
{
    CMidEvEffectVftable* vftable;
    CMidgardID eventId;
    int sequenceNumber; /**< Determines effects apply order. */
    LEventEffectCategory category;
};

struct CMidEvEffectVftable
{
    using Destructor = void(__thiscall*)(CMidEvEffect* thisptr, char flags);
    Destructor destructor;

    /** Returns true if id used by event effect equals to the objectId specified. */
    using IsIdsEqual = bool(__thiscall*)(const CMidEvEffect* thisptr, const CMidgardID* objectId);
    IsIdsEqual isIdsEqual;

    /** Returns true if effect is valid: refers to existing scenario objects. */
    using IsValid = bool(__thiscall*)(const CMidEvEffect* thisptr,
                                      const IMidgardObjectMap* objectMap);
    IsValid isValid;

    using Method3 = bool(__thiscall*)(const CMidEvEffect* thisptr, int a2);
    Method3 method3;

    /** Serializes event effect. */
    using Stream = void(__thiscall*)(CMidEvEffect* thisptr,
                                     IMidgardStreamEnv* streamEnv,
                                     IMidgardStream** stream);
    Stream stream;
};

static_assert(sizeof(CMidEvEffectVftable) == 5 * sizeof(void*),
              "CMidEvEffect vftable must have exactly 5 methods");

} // namespace game

#endif // MIDEVEFFECT_H
