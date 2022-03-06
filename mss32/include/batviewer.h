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

#ifndef BATVIEWER_H
#define BATVIEWER_H

#include "unitpositionset.h"

namespace game {

struct IBatViewer;
struct IBatViewerVftable;
struct BatViewerTargetData;
struct BatViewerTargetDataSet;
struct CMidgardID;
struct BattleMsgData;
struct BattleAttackInfo;

enum class BattleAction : int;

struct IBatViewer
{
    const IBatViewerVftable* vftable;
};

struct IBatViewerVftable
{
    using Destructor = void(__thiscall*)(IBatViewer* thisptr, bool freeMemory);
    Destructor destructor;

    /** Called on receiving CCmdBattleChooseActionMsg to show battle actions. */
    using Update = void(__thiscall*)(IBatViewer* thisptr,
                                     const BattleMsgData* battleMsgData,
                                     const CMidgardID* unitId,
                                     const Set<BattleAction>* actions,
                                     const BatViewerTargetDataSet* targetData);
    Update update;

    /** Called on receiving CCmdBattleResultMsg to show results of attack in battle. */
    using ShowAttackEffect = void(__thiscall*)(IBatViewer* thisptr,
                                               const BattleMsgData* battleMsgData,
                                               const BattleAttackInfo** attackInfo,
                                               const LAttackClass* attackClass);
    ShowAttackEffect showAttackEffect;

    /** Called on receiving CCmdBattleEndMsg to hide battle window. */
    using BattleEnd = void(__thiscall*)(IBatViewer* thisptr,
                                        const BattleMsgData* battleMsgData,
                                        const CMidgardID* a3);
    BattleEnd battleEnd;
};

static_assert(sizeof(IBatViewerVftable) == 4 * sizeof(void*),
              "IBatViewer vftable must have exactly 4 methods");

struct BatViewerTargetData
{
    CMidgardID targetGroupId;
    UnitPositionSet targetPositions;
};

static_assert(sizeof(BatViewerTargetData) == 32,
              "Size of BatViewerTargetData structure must be exactly 32 bytes");

struct BatViewerTargetDataSet
{
    BatViewerTargetData attack;
    BatViewerTargetData items[2];
};

static_assert(sizeof(BatViewerTargetDataSet) == 96,
              "Size of BatViewerTargetDataSet structure must be exactly 96 bytes");

} // namespace game

#endif // BATVIEWER_H
