/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef CMDBATTLECHOOSEACTIONMSG_H
#define CMDBATTLECHOOSEACTIONMSG_H

#include "battlemsgdata.h"
#include "commandmsg.h"
#include "sortedlist.h"

namespace game {

struct CCmdBattleChooseActionMsg
    : public CCommandMsgTempl<CommandMsgId::BattleChooseAction, CommandMsgParam::Value0>
{
    BattleMsgData battleMsgData;
    CMidgardID id;
    SortedList<int> list;
    CMidgardID id2;
    SortedList<int> list2;
    CMidgardID id3;
    SortedList<int> list3;
    CMidgardID id4;
    SortedList<int> list4;
};

static_assert(sizeof(CCmdBattleChooseActionMsg) == 4064,
              "Size of CCmdBattleChooseActionMsg structure must be exactly 4064 bytes");

static_assert(offsetof(CCmdBattleChooseActionMsg, battleMsgData) == 16,
              "CCmdBattleChooseActionMsg::battleMsgData offset must be 16 bytes");

namespace CCmdBattleChooseActionMsgApi {

struct Api
{
    using Destructor = void(__thiscall*)(CCmdBattleChooseActionMsg* thisptr);
    Destructor destructor;
};

Api& get();

CCommandMsgVftable* vftable();

} // namespace CCmdBattleChooseActionMsgApi

} // namespace game

#endif // CMDBATTLECHOOSEACTIONMSG_H
