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

#ifndef SORTEDPOINTLIST_H
#define SORTEDPOINTLIST_H

#include "mqpoint.h"
#include "sortedlist.h"

namespace game {

using SortedPointList = SortedList<CMqPoint>;

struct SortedPointListIterator
{
    char unknown[16];
};

namespace SortedPointListApi {

struct Api
{
    using Constructor = SortedPointList*(__thiscall*)(SortedPointList* thisptr,
                                                      bool* a2,
                                                      void* a3,
                                                      bool a4);
    Constructor constructor;

    using Destructor = void(__thiscall*)(SortedPointList* thisptr);
    Destructor destructor;

    using Add = SortedPointListIterator*(__thiscall*)(SortedPointList* thisptr,
                                                      SortedPointListIterator* iterator,
                                                      const CMqPoint* point);
    Add add;
};

Api& get();

} // namespace SortedPointListApi

} // namespace game

#endif // SORTEDPOINTLIST_H
