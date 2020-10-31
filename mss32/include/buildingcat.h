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

#ifndef BUILDINGCAT_H
#define BUILDINGCAT_H

#include "categories.h"
#include "categoryids.h"

namespace game {

struct LBuildingCategoryTable : public CEnumConstantTable<BuildingId>
{ };

struct LBuildingCategory : public Category<BuildingId>
{ };

namespace BuildingCategories {

struct Categories
{
    LBuildingCategory* guild;
    LBuildingCategory* heal;
    LBuildingCategory* magic;
    LBuildingCategory* unit;
};

Categories& get();

/** Returns address of LBuildingCategory::vftable used in game. */
const void* vftable();

} // namespace BuildingCategories

namespace LBuildingCategoryTableApi {

struct Api
{
    /** Creates table and building categories from contents of LBuild.dbf. */
    using Constructor = LBuildingCategoryTable*(__thiscall*)(LBuildingCategoryTable* thisptr,
                                                             const char* globalsFolderPath,
                                                             void* codeBaseEnvProxy);
    Constructor constructor;

    /** Performs initialization before reading categories. */
    using Init = void(__stdcall*)(LBuildingCategoryTable* table,
                                  void* codeBaseEnvProxy,
                                  const char* globalsFolderPath,
                                  const char* dbfFileName);
    Init init;

    /**
     * Reads building category from dbf file using category name.
     * Throws exception if category with specified name could not be found in dbf table.
     * @param[inout] category building category structure to store the result.
     * @param[in] table initialized category table to search for BuildingId constants by name.
     * @param[in] fieldName dbf table field name that represents category.
     * @param[in] dbfFileName name of dbf table to read from.
     * @returns pointer to category.
     */
    using ReadCategory = LBuildingCategory*(__stdcall*)(LBuildingCategory* category,
                                                        LBuildingCategoryTable* table,
                                                        const char* fieldName,
                                                        const char* dbfFileName);
    ReadCategory readCategory;

    /** Performs necessary cleanup after categories was read. */
    using InitDone = void*(__thiscall*)(LBuildingCategoryTable* table);
    InitDone initDone;
};

Api& get();

/** Returns address of LBuildingCategoryTable::vftable used in game. */
const void* vftable();

} // namespace LBuildingCategoryTableApi

} // namespace game

#endif // BUILDINGCAT_H
