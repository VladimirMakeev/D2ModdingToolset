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

#ifndef MIDDROPMANAGER_H
#define MIDDROPMANAGER_H

#include "d2assert.h"

namespace game {

struct IMidDropManagerVftable;
struct IMidDropSource;

struct IMidDropManager
{
    IMidDropManagerVftable* vftable;
};

struct IMidDropManagerVftable
{
    using Destructor = void(__thiscall*)(IMidDropManager* thisptr, char flags);
    Destructor destructor;

    using GetDropSource = IMidDropSource*(__thiscall*)(IMidDropManager* thisptr);
    GetDropSource getDropSource;

    using SetDropSource = void(__thiscall*)(IMidDropManager* thisptr,
                                            IMidDropSource* value,
                                            bool a3);
    SetDropSource setDropSource;

    void* method4;

    using ResetDropSource = void(__thiscall*)(IMidDropManager* thisptr);
    ResetDropSource resetDropSource;

    void* methods[9];
};

assert_vftable_size(IMidDropManagerVftable, 14);

} // namespace game

#endif // MIDDROPMANAGER_H
