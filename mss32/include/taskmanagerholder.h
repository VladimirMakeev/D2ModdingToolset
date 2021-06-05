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

#ifndef TASKMANAGERHOLDER_H
#define TASKMANAGERHOLDER_H

namespace game {

struct ITask;
struct CTaskManager;
struct ITaskManagerHolderVftable;

struct ITaskManagerHolder
{
    ITaskManagerHolderVftable* vftable;
};

struct ITaskManagerHolderVftable
{
    using Destructor = void(__thiscall*)(ITaskManagerHolder* thisptr, char flags);
    Destructor destructor;

    using GetTaskManager = CTaskManager*(__thiscall*)(ITaskManagerHolder* thisptr);
    GetTaskManager getTaskManager;

    using GetTask = ITask*(__thiscall*)(ITaskManagerHolder* thisptr);
    GetTask getTask;
};

static_assert(sizeof(ITaskManagerHolderVftable) == 3 * sizeof(void*),
              "ITaskManagerHolder vftable must have exactly 3 methods");

} // namespace game

#endif // TASKMANAGERHOLDER_H
