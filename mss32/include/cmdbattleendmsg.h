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

#ifndef CMDBATTLEENDMSG_H
#define CMDBATTLEENDMSG_H

#include "battlemsgdata.h"
#include "commandmsg.h"

namespace game {

struct CCmdBattleEndMsg : public CCommandMsgTempl<CommandMsgId::BattleEnd, CommandMsgParam::Value2>
{
    BattleMsgData battleMsgData;
    int unknown2;
    char padding[4];
};

assert_size(CCmdBattleEndMsg, 3944);
assert_offset(CCmdBattleEndMsg, battleMsgData, 16);

namespace CCmdBattleEndMsgApi {

struct Api
{
    using Destructor = void(__thiscall*)(CCmdBattleEndMsg* thisptr);
    Destructor destructor;
};

Api& get();

CCommandMsgVftable* vftable();

} // namespace CCmdBattleEndMsgApi

} // namespace game

#endif // CMDBATTLEENDMSG_H
