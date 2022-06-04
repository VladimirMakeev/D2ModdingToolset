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

#ifndef NETMSGCALLBACKS_H
#define NETMSGCALLBACKS_H

#include "d2list.h"
#include "d2set.h"
#include "smartptr.h"

namespace game {

struct CNetMsgMapEntry;
struct NetMsgCallbacks;
struct CMenuPhase;

struct NetMsgEntryNode
{
    char name[37]; /**< Raw name of CNetMsg derived class. */
    char padding[3];
    SmartPtr<CNetMsgMapEntry> ptr;
};

using NetMsgEntrySet = Set<NetMsgEntryNode>;

struct NetMsgEntryData
{
    NetMsgCallbacks** callbacks;
    NetMsgEntrySet entries;
};

using NetMsgEntryDataList = List<NetMsgEntryData**>;

struct NetMsgCallbacks
{
    NetMsgEntryDataList list;
    bool unknown;
    char padding[3];
    void* node;
    void* node2;
};

assert_size(NetMsgCallbacks, 28);

namespace NetMsgApi {

struct Api
{
    using AllocateEntryData = bool(__thiscall*)(CMenuPhase* thisptr, NetMsgEntryData*** data);
    AllocateEntryData allocateEntryData;

    using FreeEntryData = void(__stdcall*)(NetMsgEntryData** data);
    FreeEntryData freeEntryData;

    using AddEntry = void(__thiscall*)(NetMsgEntryData** thisptr, CNetMsgMapEntry* entry);
    AddEntry addEntry;
};

Api& get();

} // namespace NetMsgApi

} // namespace game

#endif // NETMSGCALLBACKS_H
