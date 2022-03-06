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

#ifndef POINTSET_H
#define POINTSET_H

#include "d2set.h"
#include "mqpoint.h"

namespace game {

using PointSet = Set<CMqPoint>;

struct PointSetIterator
{
    char unknown[16];
};

namespace PointSetApi {

struct Api
{
    using Constructor = PointSet*(__thiscall*)(PointSet* thisptr, bool* a2, void* a3, bool a4);
    Constructor constructor;

    using Destructor = void(__thiscall*)(PointSet* thisptr);
    Destructor destructor;

    using Add = PointSetIterator*(__thiscall*)(PointSet* thisptr,
                                               PointSetIterator* iterator,
                                               const CMqPoint* point);
    Add add;
};

Api& get();

} // namespace PointSetApi

} // namespace game

#endif // POINTSET_H
