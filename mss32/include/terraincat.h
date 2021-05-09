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

#ifndef TERRAINCAT_H
#define TERRAINCAT_H

#include "categories.h"

namespace game {

struct LTerrainCategoryTable : public CEnumConstantTable<TerrainId>
{ };

struct LTerrainCategory : public Category<TerrainId>
{ };

namespace TerrainCategories {

struct Categories
{
    LTerrainCategory* human;
    LTerrainCategory* heretic;
    LTerrainCategory* dwarf;
    LTerrainCategory* undead;
    LTerrainCategory* neutral;
    LTerrainCategory* elf;
};

Categories& get();

/** Returns address of LTerrainCategory::vftable used in game. */
const void* vftable();

} // namespace TerrainCategories

namespace LTerrainCategoryTableApi {

using Api = CategoryTableApi::Api<LTerrainCategoryTable, LTerrainCategory>;

Api& get();

/** Returns address of LTerrainCategoryTable::vftable used in game. */
const void* vftable();

} // namespace LTerrainCategoryTableApi

} // namespace game

#endif // TERRAINCAT_H
