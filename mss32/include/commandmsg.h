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

#ifndef COMMANDMSG_H
#define COMMANDMSG_H

#include "idlist.h"
#include "netmsg.h"

namespace game {

struct CCommandMsgVftable;

struct CCommandMsg : public CNetMsgT<CCommandMsgVftable>
{
    CMidgardID playerId;
    int unknown;
    IdList* ids;
};

static_assert(sizeof(CCommandMsg) == 16, "Size of CCommandMsg structure must be exactly 16 bytes");

enum class CommandMsgId : int
{
    BattleStart = 8,
    BattleChooseAction = 9,
    BattleResult = 10,
    BattleEnd = 11,
};

enum class CommandMsgParam : int
{
    Value0,
    Value1,
    Value2,
};

template <CommandMsgId ID, CommandMsgParam P>
struct CCommandMsgTempl : public CCommandMsg
{ };

struct CCommandMsgVftable
{
    using Destructor = void(__thiscall*)(CCommandMsg* thisptr, char flags);
    Destructor destructor;

    using Serialize = void(__thiscall*)(CCommandMsg* thisptr, CMqStream* stream);
    Serialize serialize;

    using GetId = CommandMsgId(__thiscall*)(CCommandMsg* thisptr);
    GetId getId;

    using GetParam = CommandMsgParam(__thiscall*)(CCommandMsg* thisptr);
    GetParam getParam;

    void* method5;
};

static_assert(sizeof(CCommandMsgVftable) == 5 * sizeof(void*),
              "CCommandMsg vftable must have exactly 5 methods");

namespace CCommandMsgApi {

struct Api
{
    using Destructor = void(__thiscall*)(CCommandMsg* thisptr);
    Destructor destructor;
};

Api& get();

} // namespace CCommandMsgApi

} // namespace game

#endif // COMMANDMSG_H
