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

#ifndef RACESET_H
#define RACESET_H

#include "d2set.h"
#include "racecategory.h"

namespace game {

using RaceSet = Set<LRaceCategory>;

struct RaceSetIterator
{
    char unknown[16];
};

namespace RaceSetApi {

struct Api
{
    using Clear = void(__thiscall*)(RaceSet* thisptr);
    Clear clear;

    using Add = RaceSetIterator*(__thiscall*)(RaceSet* thisptr,
                                              RaceSetIterator* iterator,
                                              LRaceCategory* raceCategory);
    Add add;
};

Api& get();

} // namespace RaceSetApi

} // namespace game

#endif // RACESET_H
