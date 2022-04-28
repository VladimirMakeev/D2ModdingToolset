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

#ifndef MODIFGROUP_H
#define MODIFGROUP_H

#include "categories.h"

namespace game {

struct LModifGroupVftable;

struct LModifGroupTable : public CEnumConstantTable<ModifierSourceId>
{ };

struct LModifGroup : public Category<ModifierSourceId>
{ };

static_assert(sizeof(LModifGroup) == 12, "Size of LModifGroup structure must be exactly 12 bytes");

namespace LModifGroupApi {

const LModifGroupVftable* vftable();

} // namespace LModifGroupApi

} // namespace game

#endif // MODIFGROUP_H
