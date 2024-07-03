/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef NOBLEACTIONCAT_H
#define NOBLEACTIONCAT_H

#include "categories.h"

namespace game {

struct LNobleActionCatTable : public CEnumConstantTable<ActionId>
{ };

struct LNobleActionCat : public Category<ActionId>
{ };

namespace NobleActionCategories {

struct Categories
{
    LNobleActionCat* poisonStack;
    LNobleActionCat* spy;
    LNobleActionCat* stealItem;
    LNobleActionCat* assassinate;
    LNobleActionCat* misfit;
    LNobleActionCat* duel;
    LNobleActionCat* poisonCity;
    LNobleActionCat* stealSpell;
    LNobleActionCat* bribe;
    LNobleActionCat* stealGold;
    LNobleActionCat* riotCity;
    LNobleActionCat* stealMerchant;
    LNobleActionCat* stealMage;
    LNobleActionCat* spyRuin;
};

Categories& get();

} // namespace NobleActionCategories

namespace LNobleActionCatTableApi {

using Api = CategoryTableApi::Api<LNobleActionCatTable, LNobleActionCat>;

Api& get();

/** Returns address of LNobleActionCatTable::vftable used in game. */
const void* vftable();

} // namespace LNobleActionCatTableApi

} // namespace game

#endif // NOBLEACTIONCAT_H
