/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef TASKOBJ_H
#define TASKOBJ_H

#include "canselect.h"
#include "taskbase.h"

namespace game {

struct CursorHandle;

namespace editor {

struct CObjectInterf;

struct CTaskObjData
{
    CObjectInterf* objectInterf;
    int unknown2;
    int unknown3;
    int unknown4;
};

assert_size(CTaskObjData, 16);

/** Base class for tasks related to scenario objects. */
struct CTaskObj : public CTaskBase
{
    ICanSelect canSelect;
    CTaskObjData* taskObjData;
};

assert_size(CTaskObj, 28);

struct CTaskObjVftable : public ITaskVftable
{
    /** Returns brush size when the task is active. */
    using GetBrushSize = CMqPoint*(__thiscall*)(const CTaskObj* thisptr, CMqPoint* size);
    GetBrushSize getBrushSize;

    using UnknownMethod = int(__thiscall*)(CTaskObj* thisptr, int a2);
    UnknownMethod unknown;

    using DoAction = bool(__thiscall*)(CTaskObj* thisptr, const CMqPoint* mapPosition);
    /** Logic depends on child class. Creates scenario objects or deletes them, moves etc. */
    DoAction doAction;

    /** Returns true if action can be performed. */
    DoAction checkActionPossible;

    using GetCursor = SmartPtr<CursorHandle>*(__thiscall*)(const CTaskObj* thisptr,
                                                           SmartPtr<CursorHandle>* cursor,
                                                           bool selectionAllowed);
    GetCursor getCursor;
};

assert_vftable_size(CTaskObjVftable, 11);

} // namespace editor

} // namespace game

#endif // TASKOBJ_H
