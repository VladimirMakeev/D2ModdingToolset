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

#ifndef NETDPLAYPLAYER_H
#define NETDPLAYPLAYER_H

#include "d2list.h"
#include "d2string.h"
#include "mqnetplayer.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace game {

struct CNetDPlaySession;
struct IMqNetSystem;
struct IMqNetReception;

/** DirectPlay implementation of network player. */
struct CNetDPlayPlayer : public IMqNetPlayer
{
    IMqNetPlayer* parent;
    CNetDPlaySession* session;
    IMqNetSystem* netSystem;
    void* unknown2;
    IMqNetReception* netReception;
    HANDLE eventHandle;
    bool stopThread;
    char padding[3];
    int directPlayId;
    String name;
    List<void*> messages;
};

static_assert(sizeof(CNetDPlayPlayer) == 68,
              "Size of CNetDPlayPlayer structure must be exactly 68 bytes");

} // namespace game

#endif // NETDPLAYPLAYER_H
