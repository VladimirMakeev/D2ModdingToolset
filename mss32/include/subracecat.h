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

#ifndef SUBRACECAT_H
#define SUBRACECAT_H

#include "categories.h"

namespace game {

struct LSubRaceCategoryTable : public CEnumConstantTable<SubRaceId>
{ };

struct LSubRaceCategory : public Category<SubRaceId>
{ };

namespace SubraceCategories {

struct Categories
{
    LSubRaceCategory* custom;
    LSubRaceCategory* human;
    LSubRaceCategory* dwarf;
    LSubRaceCategory* heretic;
    LSubRaceCategory* undead;
    LSubRaceCategory* neutral;
    LSubRaceCategory* neutralHuman;
    LSubRaceCategory* neutralElf;
    LSubRaceCategory* neutralGreenSkin;
    LSubRaceCategory* neutralDragon;
    LSubRaceCategory* neutralMarsh;
    LSubRaceCategory* neutralWater;
    LSubRaceCategory* neutralBarbarian;
    LSubRaceCategory* neutralWolf;
    LSubRaceCategory* elf;
};

Categories& get();

/** Returns address of LSubRaceCategory::vftable used in game. */
const void* vftable();

} // namespace SubraceCategories

namespace LSubRaceCategoryTableApi {

using Api = CategoryTableApi::Api<LSubRaceCategoryTable, LSubRaceCategory>;

Api& get();

/** Returns address of LSubRaceCategoryTable::vftable used in game. */
const void* vftable();

} // namespace LSubRaceCategoryTableApi

} // namespace game

#endif // SUBRACECAT_H
