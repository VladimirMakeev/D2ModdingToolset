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

#ifndef CATEGORIES_H
#define CATEGORIES_H

#include "categoryids.h"

namespace game {

/** Category table record, contains data read from database. */
template <typename T>
struct TableRecord
{
    T id;
    char* name;
};

/** Base class for all category tables. */
template <typename T>
struct CEnumConstantTable
{
    const void* vftable;
    TableRecord<T>* bgn;
    TableRecord<T>* end;
    TableRecord<T>* allocatedMemEnd;
    void* allocator;
};

/** Base class for all category types. */
template <typename T>
struct Category
{
    const void* vftable;
    CEnumConstantTable<T>* table;
    T id;
};

struct LRaceCategoryTable : public CEnumConstantTable<RaceId>
{ };

struct LRaceCategory : public Category<RaceId>
{ };

struct LAttitudesCategoryTable : CEnumConstantTable<AiAttitudeId>
{ };

struct LAttitudesCategory : public Category<AiAttitudeId>
{ };

} // namespace game

#endif // CATEGORIES_H
