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

#ifndef TASK_H
#define TASK_H

#include "d2assert.h"
#include "smartptr.h"
#include <cstdint>

namespace game {

struct ITaskVftable;
struct CMqPoint;
struct CursorHandle;
struct CTaskManager;

struct ITask
{
    ITaskVftable* vftable;
};

struct ITaskVftable
{
    using Destructor = void(__thiscall*)(ITask* thisptr, char flags);
    Destructor destructor;

    /** Called from CFullScreenInterf::onMouseMove. */
    using OnMouseMove = bool(__thiscall*)(ITask* thisptr, const CMqPoint* mousePosition);
    OnMouseMove onMouseMove;

    /** Called from CFullScreenInterf::handleMouse. */
    using HandleMouse = bool(__thiscall*)(ITask* thisptr,
                                          std::uint32_t mouseButton,
                                          const CMqPoint* mousePosition);
    HandleMouse handleMouse;

    /** Called from CFullScreenInterf::handleKeyboard. */
    using HandleKeyboard = bool(__thiscall*)(ITask* thisptr, int key, int a3);
    HandleKeyboard handleKeyboard;

    /** Called from CFullScreenInterf::getCursorAtPoint. */
    using GetCursorAtPoint = void(__thiscall*)(ITask* thisptr,
                                               SmartPtr<CursorHandle>* cursorPtr,
                                               const CMqPoint* point);
    GetCursorAtPoint getCursorAtPoint;

    using GetTaskManager = CTaskManager*(__thiscall*)(const ITask* thisptr);
    GetTaskManager getTaskManager;
};

assert_vftable_size(ITaskVftable, 6);

} // namespace game

#endif // TASK_H
