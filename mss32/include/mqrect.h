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

#ifndef MQRECT_H
#define MQRECT_H

#include "d2assert.h"

namespace game {

struct CMqPoint;

struct CMqRect
{
    struct
    {
        int left;
        int top;
        int right;
        int bottom;
    };
};

assert_size(CMqRect, 16);

namespace MqRectApi {

struct Api
{
    using Constructor = CMqRect*(__thiscall*)(CMqRect* thisptr);
    Constructor constructor;

    using PtInRect = bool(__thiscall*)(const CMqRect* thisptr, const CMqPoint* pt);
    PtInRect ptInRect;

    using GetCenter = CMqPoint*(__thiscall*)(const CMqRect* thisptr, CMqPoint* value);
    GetCenter getCenter;
};

Api& get();

} // namespace MqRectApi

} // namespace game

#endif // MQRECT_H
