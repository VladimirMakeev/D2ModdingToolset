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

#ifndef MQTHREAD_H
#define MQTHREAD_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>

namespace game {

struct CMqThreadData
{
    char unknown[80];
    HANDLE threadHandle;
    std::uint32_t threadId;
    HANDLE eventHandle;
    std::uint32_t timeoutMs;
};

static_assert(sizeof(CMqThreadData) == 96,
              "Size of CMqThreadData structure must be exactly 96 bytes");

/** Thread wrapper used in game. */
struct CMqThread
{
    void* vftable;
    CMqThreadData* data;
};

} // namespace game

#endif // MQTHREAD_H
