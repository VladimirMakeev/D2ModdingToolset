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

#ifndef NETDPLAYSERVICE_H
#define NETDPLAYSERVICE_H

#include "mqnetservice.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>

struct IDirectPlay4;

namespace game {

struct CLogFile;

/** DirectPlay implementation of net service. */
struct CNetDPlayService : public IMqNetService
{
    IDirectPlay4* directPlay;
    int unknown2;
    CLogFile* log;
    LPCRITICAL_SECTION criticalSection;
    char unknown4[36];
    std::uint32_t sessionCount;
    int unknown5;
};

static_assert(sizeof(CNetDPlayService) == 64,
              "Size of CNetDPlayService structure must be exactly 64 bytes");

} // namespace game

#endif // NETDPLAYSERVICE_H
