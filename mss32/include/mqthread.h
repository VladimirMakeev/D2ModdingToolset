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

#include "d2pair.h"
#include "d2set.h"
#include "functordispatch0.h"
#include "smartptr.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstddef>
#include <cstdint>

namespace game {

struct CMqThreadData
{
    HANDLE windowHandle;
    Set<Pair<std::uint32_t /* timerId */, SmartPtr<CBFunctorDispatch0>>> callbacks;
    std::uint32_t freeTimerId;
    char unknown[44];
    HANDLE threadHandle;
    std::uint32_t threadId;
    HANDLE eventHandle;
    std::uint32_t timeoutMs;
};

static_assert(sizeof(CMqThreadData) == 96,
              "Size of CMqThreadData structure must be exactly 96 bytes");

static_assert(offsetof(CMqThreadData, freeTimerId) == 32,
              "CMqThreadData::freeTimerId offset must be 32 bytes");

static_assert(offsetof(CMqThreadData, threadHandle) == 80,
              "CMqThreadData::threadHandle offset must be 80 bytes");

struct CMqThreadVftable;

/** Thread wrapper used in game. */
struct CMqThread
{
    CMqThreadVftable* vftable;
    CMqThreadData* data;
};

struct CMqThreadVftable
{
    using Destructor = void(__thiscall*)(CMqThread* thisptr, char flags);
    Destructor destructor;

    /**
     * Called before entering message processing loop.
     * @returns true if initial setup successfully completed.
     */
    using Start = bool(__thiscall*)(CMqThread* thisptr);
    Start start;

    /** Called after message processing loop is finished. */
    using Stop = void(__thiscall*)(CMqThread* thisptr);
    Stop stop;

    /**
     * Implements thread logic.
     * Calls start, runs processing loop, calls stop before exiting.
     * Before start, creates timer callback that will call update.
     * @returns true if start returned true.
     */
    using RunThread = bool(__thiscall*)(CMqThread* thisptr);
    RunThread runThread;

    /** Called at a constant rate specified by CMqThread::timeoutMs.  */
    using Update = void(__thiscall*)(CMqThread* thisptr);
    Update update;
};

static_assert(sizeof(CMqThreadVftable) == 5 * sizeof(void*),
              "Size of CMqThread vftable must have exactly 5 methods");

} // namespace game

#endif // MQTHREAD_H
