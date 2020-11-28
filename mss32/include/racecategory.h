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

#ifndef RACECATEGORY_H
#define RACECATEGORY_H

#include "categories.h"

namespace game {

struct LRaceCategoryTable : public CEnumConstantTable<RaceId>
{ };

struct LRaceCategory : public Category<RaceId>
{ };

namespace RaceCategories {

struct Categories
{
    LRaceCategory* human;
    LRaceCategory* dwarf;
    LRaceCategory* heretic;
    LRaceCategory* undead;
    LRaceCategory* neutral;
    LRaceCategory* elf;
};

Categories& get();

/** Returns address of LRaceCategory::vftable used in game. */
const void* vftable();

} // namespace RaceCategories

namespace LRaceCategoryTableApi {

using Api = CategoryTableApi::Api<LRaceCategoryTable, LRaceCategory>;

Api& get();

/** Returns address of LRaceCategoryTable::vftable used in game. */
const void* vftable();

} // namespace LRaceCategoryTableApi

} // namespace game

#endif // RACECATEGORY_H
