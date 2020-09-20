/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#ifndef NETDPLAYSERVICE_H
#define NETDPLAYSERVICE_H

#include "mqnetservice.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace game {

/** DirectPlay implementation of net service. */
struct CNetDPlayService : public IMqNetService
{
    int unknown;
    int unknown2;
    int unknown3;
    LPCRITICAL_SECTION criticalSection;
    char unknown[44];
};

static_assert(sizeof(CNetDPlayService) == 64,
              "Size of CNetDPlayService structure must be exactly 64 bytes");

} // namespace game

#endif // NETDPLAYSERVICE_H
