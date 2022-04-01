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

#ifndef MQANIMATOR2_H
#define MQANIMATOR2_H

namespace game {

struct IMqAnimator2Vftable;
struct IMqAnimation;

struct IMqAnimator2
{
    IMqAnimator2Vftable* vftable;
};

// Virtual table does not contain destructor
struct IMqAnimator2Vftable
{
    using HandleAnimation = bool(__thiscall*)(IMqAnimator2* thisptr, IMqAnimation* animation);

    HandleAnimation addAnimation;
    HandleAnimation addAnimation2;
    HandleAnimation removeAnimation;
    HandleAnimation removeAnimation2;
};

static_assert(sizeof(IMqAnimator2Vftable) == 4 * sizeof(void*),
              "IMqAnimator2 vftable must have exactly 4 methods");

} // namespace game

#endif // MQANIMATOR2_H
