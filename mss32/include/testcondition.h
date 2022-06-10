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

#ifndef TESTCONDITION_H
#define TESTCONDITION_H

#include "d2assert.h"

namespace game {

struct IMidgardObjectMap;
struct CMidgardID;
struct CMidEvCondition;
struct ITestConditionVftable;

/** Event condition testing interface. */
struct ITestCondition
{
    ITestConditionVftable* vftable;
};

struct ITestConditionVftable
{
    using Destructor = void(__thiscall*)(ITestCondition* thisptr, char flags);
    Destructor destructor;

    /** Returns true if event condition meets for specified player. */
    using Test = bool(__thiscall*)(const ITestCondition* thisptr,
                                   const IMidgardObjectMap* objectMap,
                                   const CMidgardID* playerId,
                                   const CMidgardID* eventId);
    Test test;
};

assert_vftable_size(ITestConditionVftable, 2);

namespace ITestConditionApi {

struct Api
{
    /** Creates test condition object from specified event condition. */
    using Create = ITestCondition*(__stdcall*)(CMidEvCondition* eventCondition,
                                               bool samePlayer,
                                               const CMidgardID* triggererStackId);
    Create create;
};

Api& get();

} // namespace ITestConditionApi

} // namespace game

#endif // TESTCONDITION_H
