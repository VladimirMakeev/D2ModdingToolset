/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef NETDPLAYSESSION_H
#define NETDPLAYSESSION_H

#include "mqnetsession.h"

namespace game {

struct CNetDPlayService;

/** DirectPlay implementation of net session. */
struct CNetDPlaySession : public IMqNetSession
{
    CNetDPlayService* directPlayService;
    char unknown[56];
};

static_assert(sizeof(CNetDPlaySession) == 64,
              "Size of CNetDPlaySession structure must be exactly 64 bytes");

} // namespace game

#endif // NETDPLAYSESSION_H
