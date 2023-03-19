/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#ifndef AITACTICACTION_H
#define AITACTICACTION_H

#include "d2assert.h"

namespace game {

struct CMqPoint;
struct CMidgardID;
struct IMidgardObjectMap;
struct CMidServerLogicData;
struct IAiTacticInternal;
struct IAiTacticActionVftable;

struct IAiTacticAction
{
    IAiTacticActionVftable* vftable;
};

struct IAiTacticActionVftable
{
    using Destructor = void(__thiscall*)(IAiTacticAction* thisptr, char flags);
    Destructor destructor;

    using GetUnknown = int(__thiscall*)(const IAiTacticAction* thisptr);
    GetUnknown getUnknown;

    using GetStackId = const CMidgardID*(__thiscall*)(const IAiTacticAction* thisptr);
    GetStackId getStackId;

    using GetTargetId = const CMidgardID*(__thiscall*)(const IAiTacticAction* thisptr);
    GetTargetId getTargetId;

    using GetTargetPosition = const CMqPoint*(__thiscall*)(const IAiTacticAction* thisptr);
    GetTargetPosition getTargetPosition;

    void* methods[2];

    using Execute = void(__thiscall*)(IAiTacticAction* thisptr,
                                      IMidgardObjectMap* objectMap,
                                      CMidServerLogicData* serverLogicData,
                                      IAiTacticInternal* tacticInternal);
    Execute execute;

    void* method8;
};

assert_vftable_size(IAiTacticActionVftable, 9);

} // namespace game

#endif // AITACTICACTION_H
