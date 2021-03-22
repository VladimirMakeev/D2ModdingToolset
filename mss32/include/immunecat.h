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

#ifndef IMMUNECAT_H
#define IMMUNECAT_H

#include "categories.h"

namespace game {

struct LImmuneCatTable : CEnumConstantTable<ImmuneId>
{ };

struct LImmuneCat : public Category<ImmuneId>
{ };

namespace ImmuneCategories {

struct Categories
{
    LImmuneCat* notimmune;
    LImmuneCat* once;
    LImmuneCat* always;
};

Categories& get();

/** Returns address of LImmuneCat::vftable used in game. */
const void* vftable();

} // namespace ImmuneCategories

namespace LImmuneCatTableApi {

using Api = CategoryTableApi::Api<LImmuneCatTable, LImmuneCat>;

Api& get();

/** Returns address of LImmuneCatTable::vftable used in game. */
const void* vftable();

} // namespace LImmuneCatTableApi

} // namespace game

#endif // IMMUNECAT_H
