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

#ifndef AILOGIC_H
#define AILOGIC_H

#include "idlist.h"
#include "midgardid.h"
#include "smartptr.h"

namespace game {

struct CMidgardScenarioMap;
struct CMidServerLogicData;
struct CAiBlockingStacks;
struct CAiLog;
struct IAiState;

struct AiLogicUnknown
{
    CMidgardScenarioMap* objectMap;
    CMidServerLogicData* serverLogic;
    CMidgardID playerId;
};

assert_size(AiLogicUnknown, 12);

using AiStatePtr = SmartPtr<IAiState>;
using AiStateList = List<AiStatePtr>;
using AiStateListNode = ListNode<AiStatePtr>;

struct AiLogicUnknown2
{
    CMidgardScenarioMap* objectMap;
    AiStateList aiStateList;
    bool unknown;
    char padding[3];
    AiStateListNode* aiStateListNodePtr;
    int unknown3;
    CAiBlockingStacks* aiBlockingStacks;
    IdList list2;
    IdList list3;
    int unknown5;
    bool unknown6;
    char padding2[3];
    CMidgardID playerId;
    CMidServerLogicData* serverLogic;
    AiLogicUnknown* aiLogicUnknown;
    CAiLog* aiLog;
};

assert_size(AiLogicUnknown2, 92);

struct AiLogicUnknown4
{
    CMidgardScenarioMap* objectMap;
    CMidServerLogicData* serverLogic;
    CMidgardID playerId;
    AiLogicUnknown2* logicUnknown2;
    AiLogicUnknown* logicUnknown;
    IdList* playerIdList;
};

assert_size(AiLogicUnknown4, 24);

struct AiLogicUnknown3
{
    CMidgardID playerId;
    AiLogic* aiLogic;
    AiLogicUnknown2* logicUnknown2;
    AiLogicUnknown4* logicUnknown4;
};

assert_size(AiLogicUnknown3, 16);

struct AiLogic
{
    CMidgardScenarioMap* objectMap;
    CMidgardID playerId;
    CMidServerLogicData* serverLogic;
    int unknown;
    AiLogicUnknown3* logicUnknown3;
    AiLogicUnknown2* logicUnknown2;
    AiLogicUnknown* logicUnknown;
};

assert_size(AiLogic, 28);

} // namespace game

#endif // AILOGIC_H
