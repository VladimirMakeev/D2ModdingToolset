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

#ifndef MIDCOMMANDQUEUE2_H
#define MIDCOMMANDQUEUE2_H

#include "idlist.h"
#include "uievent.h"

namespace game {

struct CCommandMsg;
struct CoreCommandUpdate;
struct CCommandCanIgnore;
struct NetMsgEntryData;

struct CMidCommandQueue2
{
    struct INotifyCQVftable;

    struct INotifyCQ
    {
        INotifyCQVftable* vftable;
    };

    /** Net message map. */
    struct CNMMap
    {
        CMidCommandQueue2* commandQueue;
        NetMsgEntryData** netMsgEntryData;
    };

    CoreCommandUpdate* commandUpdate;
    CCommandCanIgnore* commandCanIgnore;
    CNMMap* netMessageMap;
    List<CCommandMsg*> commandsList;
    bool unknown6;
    bool unknown7;
    char padding[2];
    List<INotifyCQ*> notifyList;
    ListIterator<INotifyCQ*> currentNotify;
    bool unknown14;
    char padding2[3];
    unsigned int commandQueueMsgId;
    UiEvent commandQueueEvent;
};

assert_size(CMidCommandQueue2, 92);
assert_size(CMidCommandQueue2::CNMMap, 8);

struct CMidCommandQueue2::INotifyCQVftable
{
    using Destructor = void(__thiscall*)(CMidCommandQueue2::INotifyCQ* thisptr, char flags);
    Destructor destructor;

    using Notify = void(__thiscall*)(CMidCommandQueue2::INotifyCQ* thisptr);
    Notify notify;
};

assert_vftable_size(CMidCommandQueue2::INotifyCQVftable, 2);

} // namespace game

#endif // MIDCOMMANDQUEUE2_H
