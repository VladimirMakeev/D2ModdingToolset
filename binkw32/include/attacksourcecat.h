/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#ifndef ATTACKSOURCECAT_H
#define ATTACKSOURCECAT_H

#include "categories.h"

namespace game {

struct LAttackSourceTable : public CEnumConstantTable<AttackSourceId>
{ };

struct LAttackSource : public Category<AttackSourceId>
{ };

namespace AttackSourceCategories {

struct Categories
{
    LAttackSource* weapon;
    LAttackSource* mind;
    LAttackSource* life;
    LAttackSource* death;
    LAttackSource* fire;
    LAttackSource* water;
    LAttackSource* air;
    LAttackSource* earth;
};

Categories& get();

} // namespace AttackSourceCategories

} // namespace game

#endif // ATTACKSOURCECAT_H
