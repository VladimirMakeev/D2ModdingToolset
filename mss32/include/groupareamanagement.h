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

#ifndef GROUPAREAMANAGEMENT_H
#define GROUPAREAMANAGEMENT_H

#include "d2vector.h"
#include "mqrect.h"

namespace game {

struct CGroupAreaManagementVftable;

struct CGroupAreaManagementData
{
    Vector<CMqRect> unitAreas;
    CMqRect groupArea;
    int leftSide;
};

assert_size(CGroupAreaManagementData, 36);

struct CGroupAreaManagement
{
    CGroupAreaManagementVftable* vftable;
    CGroupAreaManagementData* data;
};

assert_size(CGroupAreaManagement, 8);

struct CGroupAreaManagementVftable
{
    using Destructor = void(__thiscall*)(CGroupAreaManagement* thisptr, char flags);
    Destructor destructor;

    /** Returns unit area index that contains specified point or -1. */
    using GetAreaIndex = int(__thiscall*)(CGroupAreaManagement* thisptr, const CMqPoint* position);
    GetAreaIndex getAreaIndex;

    /** Returns unit area by unit index in group. */
    using GetUnitArea = CMqRect*(__thiscall*)(CGroupAreaManagement* thisptr,
                                              CMqRect* unitArea,
                                              const int* index);
    GetUnitArea getUnitArea;

    using GetLeftSide = int(__thiscall*)(const CGroupAreaManagement* thisptr);
    GetLeftSide getLeftSide;
};

assert_vftable_size(CGroupAreaManagementVftable, 4);

} // namespace game

#endif // GROUPAREAMANAGEMENT_H
