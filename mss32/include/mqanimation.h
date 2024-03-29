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

#ifndef MQANIMATION_H
#define MQANIMATION_H

#include "d2assert.h"

namespace game {

struct IMqAnimationVftable;

struct IMqAnimation
{
    IMqAnimationVftable* vftable;
};

struct IMqAnimationVftable
{
    using Destructor = void(__thiscall*)(IMqAnimation* thisptr, char flags);
    Destructor destructor;

    using Update = void(__stdcall*)(IMqAnimation* thisptr);
    Update update;
};

assert_vftable_size(IMqAnimationVftable, 2);

} // namespace game

#endif // MQANIMATION_H
