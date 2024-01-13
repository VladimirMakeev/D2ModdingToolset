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

#ifndef SITECATEGORIES_H
#define SITECATEGORIES_H

#include "categories.h"

namespace game {

struct LSiteCategoryTable : public CEnumConstantTable<SiteId>
{ };

struct LSiteCategory : public Category<SiteId>
{ };

namespace SiteCategories {

struct Categories
{
    LSiteCategory* merchant;
    LSiteCategory* mageTower;
    LSiteCategory* mercenaries;
    LSiteCategory* trainer;
};

Categories& get();

/** Returns address of LSiteCategory::vftable used in game. */
const void* vftable();

} // namespace SiteCategories

namespace LSiteCategoryTableApi {

using Api = CategoryTableApi::Api<LSiteCategoryTable, LSiteCategory>;

Api& get();

/** Returns address of LSiteCategoryTable::vftable used in game. */
const void* vftable();

} // namespace LSiteCategoryTableApi

} // namespace game

#endif // SITECATEGORIES_H
