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

#ifndef DBTABLE_H
#define DBTABLE_H

#include "d2pair.h"
#include "mq_c_s.h"

namespace game {

struct String;
struct CMidgardID;
struct TextAndId;
struct GlobalData;
struct CUmModifier;
struct LBuildingCategory;
struct LBuildingCategoryTable;
struct LUnitBranch;
struct LUnitBranchTable;
struct LModifGroup;
struct LModifGroupTable;
struct LAttackClass;
struct LAttackClassTable;
struct LAttackSource;
struct LAttackSourceTable;
struct LAttackReach;
struct LAttackReachTable;

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
     * Finds LModifGroup by id in dbf table field.
     * Populates category fields when found. Throws exception if category could not be found.
     * @param[inout] category category to store search result.
     * @param[in] dbTable table object to search.
     * @param[in] fieldName table field to read category id for search from.
     * @param[in] table table where to search category by id.
     * @returns pointer to category.
     */
    using FindModifGroupCategory = LModifGroup*(__stdcall*)(LModifGroup* category,
                                                            const CDBTable* dbTable,
                                                            const char* fieldName,
                                                            const LModifGroupTable* table);
    FindModifGroupCategory findModifGroupCategory;

    /**
     * Read string from dbf table field (max length is 256 including null terminator).
     * Throws exception if failed.
     * @param[inout] value string to store result.
     * @param[in] dbTable table object to read from.
     * @param[in] fieldName table field to read from.
     */
    using ReadString = void(__stdcall*)(String* value,
                                        const CDBTable* thisptr,
                                        const char* fieldName);
    ReadString readString;

    using ReadModifier = void(__stdcall*)(CUmModifier** value,
                                          const CMidgardID* modifierId,
                                          const LModifGroup* group,
                                          const char* globalsFolderPath,
                                          void* codeBaseEnvProxy,
                                          const GlobalData** globalData);
    ReadModifier readModifier;

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
     * Throws exception related to missing value in dbf table.
     * @param[in] tableName name of the table that misses value.
     * @param[in] value string representation of missing entity.
     */
    using MissingValueException = void(__stdcall*)(const char* tableName, const char* value);
    MissingValueException missingValueException;

    /**
     * Throws exception related to duplicate records in dbf table.
     * @param[in] dbTable table object containing duplicates.
     * @param[in] id id of duplicate entity related to table record.
     */
    using DuplicateRecordException = void(__stdcall*)(const CDBTable* dbTable, CMidgardID* id);
    DuplicateRecordException duplicateRecordException;

    /**
     * Read id from dbf table field.
     * Checks if id string in table contains valid id. Throws exception in case of invalid id.
     * @param[inout] id id to store result.
     * @param[in] dbTable table object to read from.
     * @param[in] fieldName table field to read from.
     */
    using ReadId = void(__stdcall*)(CMidgardID* id, const CDBTable* dbTable, const char* fieldName);
    ReadId readId;

    using ReadText = void(__stdcall*)(TextAndId* text,
                                      const CDBTable* dbTable,
                                      const char* fieldName,
                                      const mq_c_s<Pair<CMidgardID, char*>>* texts);
    ReadText readText;

    using FindAttackClass = LAttackClass*(__stdcall*)(LAttackClass* attackClass,
                                                      const CDBTable* dbTable,
                                                      const char* fieldName,
                                                      const LAttackClassTable* table);
    FindAttackClass findAttackClass;

    using FindAttackSource = LAttackSource*(__stdcall*)(LAttackSource* attackSource,
                                                        const CDBTable* dbTable,
                                                        const char* fieldName,
                                                        const LAttackSourceTable* table);
    FindAttackSource findAttackSource;

    using FindAttackReach = LAttackReach*(__stdcall*)(LAttackReach* attackReach,
                                                      const CDBTable* dbTable,
                                                      const char* fieldName,
                                                      const LAttackReachTable* table);
    FindAttackReach findAttackReach;

    using ReadAttackIntValue = void(__stdcall*)(int* value,
                                                const CDBTable* dbTable,
                                                const char* fieldName,
                                                const CMidgardID* attackId);
    ReadAttackIntValue readInitiative;

    using ReadPower = void(__thiscall*)(int* thisptr,
                                        int* power,
                                        const CDBTable* dbTable,
                                        const char* fieldName,
                                        const CMidgardID* attackId);
    ReadPower readPower;

    ReadAttackIntValue readDamage;
    ReadAttackIntValue readHeal;

    using ReadAttackLevel = void(__stdcall*)(int* level,
                                             const CDBTable* dbTable,
                                             const char* fieldName,
                                             const CMidgardID* attackId,
                                             const LAttackClass* attackClass);
    ReadAttackLevel readAttackLevel;

    using ReadBoolValue = void(__stdcall*)(bool* value,
                                           const CDBTable* dbTable,
                                           const char* fieldName);
    ReadBoolValue readInfinite;

    using ReadIntWithBoundsCheck = void(__stdcall*)(int* value,
                                                    const CDBTable* dbTable,
                                                    const char* fieldName,
                                                    int minBound,
                                                    int maxBound);
    ReadIntWithBoundsCheck readIntWithBoundsCheck;

    ReadBoolValue readBool;

    using GetName = const char*(__thiscall*)(const CDBTable* thisptr);
    GetName getName;

    using Eof = bool(__thiscall*)(const CDBTable* thisptr);
    Eof eof;

    using Next = bool(__thiscall*)(CDBTable* thisptr);
    Next next;
};

Api& get();

} // namespace CDBTableApi

} // namespace game

#endif // DBTABLE_H
