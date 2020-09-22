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

#ifndef DBTABLE_H
#define DBTABLE_H

namespace game {

struct LBuildingCategory;
struct LBuildingCategoryTable;
struct LUnitBranch;
struct LUnitBranchTable;
struct CMidgardID;

/** One of the classes in game that wraps dbf access logic. */
struct CDBTable;

namespace CDBTableApi {

struct Api
{
    /**
     * Finds LBuildingCategory by id in dbf table field.
     * Populates category fields when found. Throws exception if category could not be found.
     * @param[inout] category category to store search result.
     * @param[in] dbTable table object to search.
     * @param[in] fieldName table field to read category id for search from.
     * @param[in] table table where to search category by id.
     * @returns pointer to category.
     */
    using FindBuildingCategory =
        LBuildingCategory*(__stdcall*)(LBuildingCategory* category,
                                       const CDBTable* dbTable,
                                       const char* fieldName,
                                       const LBuildingCategoryTable* table);
    FindBuildingCategory findBuildingCategory;

    /**
     * Finds LUnitBranch by id in dbf table field.
     * Populates category fields when found. Throws exception if category could not be found.
     * @param[inout] category category to store search result.
     * @param[in] dbTable table object to search.
     * @param[in] fieldName table field to read category id for search from.
     * @param[in] table table where to search category by id.
     * @returns pointer to category.
     */
    using FindUnitBranchCategory = LUnitBranch*(__stdcall*)(LUnitBranch* category,
                                                            const CDBTable* dbTable,
                                                            const char* fieldName,
                                                            const LUnitBranchTable* table);
    FindUnitBranchCategory findUnitBranchCategory;

    /**
     * Reads unit level from dbf table field.
     * Checks if unit level value does not exceed [unitMinLevel : unitMaxLevel] range,
     * [1 : 5] by default. Throws exception if level value exceeds range.
     * @param[inout] level location to store result.
     * @param[in] dbTable table object to read from.
     * @param[in] fieldName table field to read from.
     */
    using ReadUnitLevel = void(__stdcall*)(int* level,
                                           const CDBTable* dbTable,
                                           const char* fieldName);
    ReadUnitLevel readUnitLevel;

    /**
     * Throws exception related to duplicate records in dbf table.
     * @param[in] dbTable table object containing duplicates.
     * @param[in] id id of duplicate entity related to table record.
     */
    using DuplicateRecordException = void(__stdcall*)(const CDBTable* dbTable, CMidgardID* id);
    DuplicateRecordException duplicateRecordException;
};

Api& get();

} // namespace CDBTableApi

} // namespace game

#endif // DBTABLE_H
