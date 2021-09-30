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

#ifndef NETDPLAYSESSENUM_H
#define NETDPLAYSESSENUM_H

#include "d2string.h"
#include "mqnetsessenum.h"
#include <guiddef.h>

namespace game {

struct CNetDPlayService;

/** Common session info wrapper for Direct Play. */
struct CNetDPlaySessEnum : public IMqNetSessEnum
{
    CNetDPlayService* service;
    GUID instanceGuid;
    GUID appGuid;
    String sessionName;
    int playersCount;
    int maxPlayers;
    bool requiresPassword;
    char padding[3];
};

static_assert(sizeof(CNetDPlaySessEnum) == 68,
              "Size of CNetDPlaySessEnum structure must be exactly 68 bytes");

} // namespace game

#endif // NETDPLAYSESSENUM_H
