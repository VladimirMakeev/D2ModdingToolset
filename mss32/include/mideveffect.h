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
struct String;

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

assert_vftable_size(CMidEvEffectVftable, 5);

namespace CMidEvEffectApi {

struct Api
{

    using CreateFromCategory = CMidEvEffect*(__stdcall*)(const CMidgardID* eventId,
                                                         const LEventEffectCategory* category);
    CreateFromCategory createFromCategory;

    /**
     * Returns string with current effect settings to show in effects list box.
     * Used in Scenario Editor only.
     */
    using GetInfoString = void(__stdcall*)(String* info,
                                           const IMidgardObjectMap* objectMap,
                                           const CMidEvEffect* eventEffect);
    GetInfoString getInfoString;

    using GetDescription = void(__stdcall*)(String* description,
                                            const LEventEffectCategory* category);

    /**
     * Returns brief effect description by specified category
     * Used in Scenario Editor only.
     */
    GetDescription getBrief;

    /**
     * Returns full effect description by specified category
     * Used in Scenario Editor only.
     */
    GetDescription getDescription;
};

Api& get();

} // namespace CMidEvEffectApi

} // namespace game

#endif // MIDEVEFFECT_H
