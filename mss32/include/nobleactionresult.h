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

#ifndef NOBLEACTIONRESULT_H
#define NOBLEACTIONRESULT_H

#include "d2list.h"

namespace game {

struct INobleActionResultVftable;
struct CMidgardID;
struct IMidgardObjectMap;
struct IEventEffect;
struct LNobleActionCat;

struct INobleActionResult
{
    INobleActionResultVftable* vftable;
};

assert_size(INobleActionResult, 4);

struct INobleActionResultVftable
{
    using Destructor = void(__thiscall*)(INobleActionResult* thisptr, char flags);
    Destructor destructor;

    /** Returns integer value that is specific to action result. */
    using GetValue = int(__thiscall*)(const INobleActionResult* thisptr);
    GetValue getValue;

    /** Returns id value that is specific to action result. */
    using GetId = const CMidgardID*(__thiscall*)(const INobleActionResult* thisptr);
    GetId getId;

    /** Applies noble action result and returns true on success. */
    using Apply = bool(__thiscall*)(INobleActionResult* thisptr,
                                    IMidgardObjectMap* objectMap,
                                    List<IEventEffect*>* effects,
                                    const CMidgardID* stackId,
                                    const CMidgardID* targetObjectId);
    Apply apply;
};

assert_vftable_size(INobleActionResultVftable, 4);

namespace NobleActionsApi {

struct Api
{
    /** Creates noble action result according to category specified. */
    using Create = INobleActionResult*(__stdcall*)(IMidgardObjectMap* objectMap,
                                                   const LNobleActionCat* actionCategory,
                                                   const CMidgardID* targetObjectId,
                                                   const CMidgardID* id);
    Create create;
};

Api& get();

} // namespace NobleActionsApi

} // namespace game

#endif // NOBLEACTIONRESULT_H
