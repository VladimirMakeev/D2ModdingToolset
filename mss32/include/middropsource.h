/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef MIDDROPSOURCE_H
#define MIDDROPSOURCE_H

#include "cursorhandle.h"

namespace game {

struct IMidDropSourceVftable;

struct IMidDropSource
{
    IMidDropSourceVftable* vftable;
};

struct IMidDropSourceVftable
{
    using Destructor = void(__thiscall*)(IMidDropSource* thisptr, char flags);
    Destructor destructor;

    using GetCursorHandle =
        SmartPtr<CursorHandle>*(__thiscall*)(IMidDropSource* thisptr,
                                             bool dragDropAllowed,
                                             SmartPtr<CursorHandle>* cursorHandle);
    GetCursorHandle getCursorHandle;

    void* method2;
    void* method3;

    using IsPointOverButton = bool(__thiscall*)(IMidDropSource* thisptr, const CMqPoint* point);
    IsPointOverButton isPointOverButton;

    using ResetToggleButton = bool(__thiscall*)(IMidDropSource* thisptr, int a2);
    ResetToggleButton resetToggleButton;

    void* method6;

    using Method7 = int(__thiscall*)(IMidDropSource* thisptr);
    Method7 method7;

    void* method8;

    using Method9 = int(__thiscall*)(IMidDropSource* thisptr, bool buttonPressed);
    Method9 method9;
};

assert_vftable_size(IMidDropSourceVftable, 10);

} // namespace game

#endif // MIDDROPSOURCE_H
