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

#ifndef SORTEDCAPITALRACELIST_H
#define SORTEDCAPITALRACELIST_H

#include "capitaldatlist.h"
#include "racecategory.h"
#include "sortedlist.h"

namespace game {

struct CapitalRaceRecord
{
    LRaceCategory race;
    CapitalDatList* list;
};

static_assert(sizeof(CapitalRaceRecord) == 16,
              "Size of CapitalRaceRecord structure must be exactly 16 bytes");

using SortedCapitalRaceList = SortedList<CapitalRaceRecord>;

struct SortedCapitalRaceListIterator
{
    char unknown[16];
};

namespace SortedCapitalRaceListApi {

struct Api
{
    using Constructor = SortedCapitalRaceList*(__thiscall*)(SortedCapitalRaceList* thisptr);
    Constructor constructor;

    using Add = SortedCapitalRaceListIterator*(__thiscall*)(SortedCapitalRaceListIterator* thisptr,
                                                            SortedCapitalRaceListIterator* iterator,
                                                            const LRaceCategory* race,
                                                            CapitalDatList** list);
    Add add;
};

Api& get();

} // namespace SortedCapitalRaceListApi

} // namespace game

#endif // SORTEDCAPITALRACELIST_H
