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

#ifndef SORTEDRACELIST_H
#define SORTEDRACELIST_H

#include "d2set.h"
#include "racecategory.h"

namespace game {

using SortedRaceList = Set<LRaceCategory>;

struct SortedRaceListIterator
{
    char unknown[16];
};

namespace SortedRaceListApi {

struct Api
{
    using Clear = void(__thiscall*)(SortedRaceList* thisptr);
    Clear clear;

    using Add = SortedRaceListIterator*(__thiscall*)(SortedRaceList* thisptr,
                                                     SortedRaceListIterator* iterator,
                                                     LRaceCategory* raceCategory);
    Add add;
};

Api& get();

} // namespace SortedRaceListApi

} // namespace game

#endif // SORTEDRACELIST_H
