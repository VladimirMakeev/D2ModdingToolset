/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef AIOPERATIONAL_H
#define AIOPERATIONAL_H

#include "aistate.h"
#include "idlist.h"

namespace game {

struct IMidgardObjectMap;
struct CMidServerLogicData;
struct CAiLog;
struct CAiReactionOper;

struct IAiOperationalInternalVftable;

struct IAiOperationalInternal
{
    IAiOperationalInternalVftable* vftable;
};

struct IAiOperationalInternalVftable
{
    using Destructor = void(__thiscall*)(IAiOperationalInternal* thisptr, char flags);
    Destructor destructor;

    void* methods[3];
};

struct CAiOperational
    : public IAiState
    , public IAiOperationalInternal
{
    IMidgardObjectMap* objectMap;
    CMidgardID playerId;
    CMidServerLogicData* serverLogic;
    CAiLog* aiLog;
    IdList* list;
    IdList* list2;
    CAiReactionOper* reactionOper;
    int unknown;
    bool unknown2;
    char padding[3];
    int unknown3;
};

assert_size(CAiOperational, 48);

} // namespace game

#endif // AIOPERATIONAL_H
