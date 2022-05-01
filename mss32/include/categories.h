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

struct LAttitudesCategoryTable : CEnumConstantTable<AiAttitudeId>
{ };

struct LAttitudesCategory : public Category<AiAttitudeId>
{ };

static const int emptyCategoryId = -1;

namespace CategoryTableApi {

template <typename Table, typename Category>
struct Api
{
    /** Creates table and categories from contents of dbf file. */
    using Constructor = Table*(__thiscall*)(Table* thisptr,
                                            const char* globalsFolderPath,
                                            void* codeBaseEnvProxy);
    Constructor constructor;

    /** Performs initialization before reading categories. */
    using Init = void(__stdcall*)(Table* table,
                                  void* codeBaseEnvProxy,
                                  const char* globalsFolderPath,
                                  const char* dbfFileName);
    Init init;

    /**
     * Reads categories from dbf file using category name.
     * Throws exception if category with specified name could not be found in dbf table.
     * @param[inout] category category structure to store the result.
     * @param[in] table initialized category table to search for id constants by name.
     * @param[in] fieldName dbf table field name that represents category.
     * @param[in] dbfFileName name of dbf table to read from.
     * @returns pointer to category.
     */
    using ReadCategory = Category*(__stdcall*)(Category* category,
                                               Table* table,
                                               const char* fieldName,
                                               const char* dbfFileName);
    ReadCategory readCategory;

    /** Performs necessary cleanup after categories was read. */
    using InitDone = void*(__thiscall*)(Table* table);
    InitDone initDone;

    /** Looks for category with the specified id, otherwise initializes the value with null table
     * and id = -1. */
    using FindCategoryById = Category*(__thiscall*)(Table* thisptr,
                                                    Category* value,
                                                    const int* categoryId);
    FindCategoryById findCategoryById;
};

} // namespace CategoryTableApi

} // namespace game

#endif // CATEGORIES_H
