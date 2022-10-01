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

#ifndef MIDANIM2_H
#define MIDANIM2_H

#include "d2assert.h"

namespace game {

struct IMidAnim2Vftable;

struct IMidAnim2
{
    IMidAnim2Vftable* vftable;
};

struct IMidAnim2Vftable
{
    using Destructor = void(__thiscall*)(IMidAnim2* thisptr, char flags);
    Destructor destructor;

    using ChangeFrame = void(__thiscall*)(IMidAnim2* thisptr);
    ChangeFrame changeFrame;
    ChangeFrame changeFrame2;

    using Hide = void(__thiscall*)(IMidAnim2* thisptr);
    Hide hide;

    using IsOver = bool(__thiscall*)(const IMidAnim2* thisptr);
    IsOver isOver;

    using GetSmoothScroll = bool(__thiscall*)(const IMidAnim2* thisptr);
    GetSmoothScroll getSmoothScroll;
};

assert_vftable_size(IMidAnim2Vftable, 6);

} // namespace game

#endif // MIDANIM2_H
