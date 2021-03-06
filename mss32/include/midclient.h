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

#ifndef MIDCLIENT_H
#define MIDCLIENT_H

#include "linkedlist.h"
#include "midclientcore.h"
#include "midcommandqueue2.h"
#include "midgardid.h"

namespace game {

struct CPhase;

struct CMidClientData
{
    CPhase* phase;
    int unknown2;
    int unknown3;
    int unknown4;
    int unknown5;
    int unknown6;
    LinkedList<CMidgardID> list;
    LinkedList<CMidgardID> list2;
    LinkedList<CMidgardID> list3;
    int unknown7;
    int unknown8;
    int unknown9;
    int unknown10;
    char unknown11[48];
};

static_assert(sizeof(CMidClientData) == 136,
              "Size of CMidClientData structure must be exactly 136 bytes");

struct CMidClient : public CMidCommandQueue2::INotifyCQ
{
    CMidClientCore core;
    CMidClientData* data;
};

static_assert(sizeof(CMidClient) == 16, "Size of CMidClient structure must be exactly 16 bytes");

} // namespace game

#endif // MIDCLIENT_H
