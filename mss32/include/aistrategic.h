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

#ifndef AISTRATEGIC_H
#define AISTRATEGIC_H

#include "aistate.h"
#include "currency.h"
#include "d2set.h"
#include "idlist.h"

namespace game {

struct IAiStratInternalVftable;
struct IMidgardObjectMap;
struct CMidServerLogicData;
struct CAiBlockingStacks;
struct CAiLog;
struct AiLogicUnknown;
struct IAiStratPlayer;
struct CAiReactionStrat;

struct IAiStratInternal
{
    IAiStratInternalVftable* vftable;
};

struct CAiStrategicData
{
    IMidgardObjectMap* objectMap;
    CMidgardID* playerId;
    Bank bank;
    Bank bank2;
    Set<void*> set;
};

assert_size(CAiStrategicData, 60);

struct CAiStrategicData2
{
    IdList list;
    IMidgardObjectMap* objectMap;
};

assert_size(CAiStrategicData2, 20);

struct CAiStrategic
    : public IAiState
    , public IAiStratInternal
{
    IMidgardObjectMap* objectMap;
    CMidgardID playerId;
    CMidServerLogicData* serverLogic;
    CAiBlockingStacks* blockingStats;
    CAiLog* aiLog;
    AiLogicUnknown* logicUnknown;
    IAiStratPlayer* aiStratPlayer;
    CAiStrategicData* data;
    CAiReactionStrat* reactionStrat;
    List<void*> list;
    CAiStrategicData2* data2;
    bool unknown;
    char padding[3];
    int unknown2;
    int unknown3;
    IdList list2;
    bool unknown4;
    char padding2[3];
    int unknown5;
    int unknown6;
    IdList list3;
    bool unknown7;
    char padding3[3];
    int unknown8;
    int unknown9;
    IdList* list4;
};

assert_size(CAiStrategic, 136);

} // namespace game

#endif // AISTRATEGIC_H
