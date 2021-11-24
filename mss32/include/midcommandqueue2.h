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

struct CoreCommandUpdate;
struct CCommandCanIgnore;
struct NetMsgEntryData;

struct CMidCommandQueue2
{
    struct INotifyCQ
    {
        void* vftable;
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
    IdList list;
    int unknown6;
    IdList list2;
    int unknown11;
    int unknown12;
    int unknown13;
    int unknown14;
    unsigned int commandQueueMsgId;
    UiEvent commandQueueEvent;
};

static_assert(sizeof(CMidCommandQueue2) == 92,
              "Size of CMidCommandQueue2 structure must be exactly 92 bytes");

static_assert(sizeof(CMidCommandQueue2::CNMMap) == 8,
              "Size of CMidCommandQueue2::CNMMap structure must be exactly 8 bytes");

} // namespace game

#endif // MIDCOMMANDQUEUE2_H
