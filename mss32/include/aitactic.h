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

#ifndef AITACTIC_H
#define AITACTIC_H

#include "aistate.h"
#include "idlist.h"

namespace game {

struct IAiTacticInternalVftable;
struct IMidgardObjectMap;
struct CMidServerLogicData;
struct CAiReactionTactic;
struct CAiReactionNull;

struct IAiTacticInternal
{
    IAiTacticInternalVftable* vftable;
};

struct CAiTactic
    : public IAiState
    , public IAiTacticInternal
{
    IMidgardObjectMap* objectMap;
    CMidgardID playerId;
    CMidServerLogicData* serverLogic;
    CAiReactionTactic* reactionTactic;
    CAiReactionNull reactionNull;
    SortedIntList list;
    MapSizeBigArrayPair pair;
    SortedIntList list2;
    CAiBlockingStacks* blockingStacks;
    Bank bank;
    AiLogicUnknown* logicUnknown;
    CAiLog* aiLog;
    int unknown3;
    IdList list3;
    IdList list4;
    IdList list5;
    IdList list6;
    int unknown;
    CAiTacticData* data;
    int unknown4;
    CMidgardID unknownId;
    CMidServerLogicData2* serverLogic2;
    IdList list7;
    bool unknown5;
    bool unknown6;
    char padding[2];
    SortedIntList list8;
    IdList* list9;
    IdList* list10;
};

} // namespace game

#endif // AITACTIC_H
