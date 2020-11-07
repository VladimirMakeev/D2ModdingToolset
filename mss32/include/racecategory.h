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

#ifndef RACECATEGORY_H
#define RACECATEGORY_H

#include "categories.h"

namespace game {

struct LRaceCategoryTable : public CEnumConstantTable<RaceId>
{ };

struct LRaceCategory : public Category<RaceId>
{ };

namespace RaceCategories {

struct Categories
{
    LRaceCategory* human;
    LRaceCategory* dwarf;
    LRaceCategory* heretic;
    LRaceCategory* undead;
    LRaceCategory* neutral;
    LRaceCategory* elf;
};

Categories& get();

/** Returns address of LRaceCategory::vftable used in game. */
const void* vftable();

} // namespace RaceCategories

namespace LRaceCategoryTableApi {

struct Api
{
    /** Creates table and race categories from contents of LRace.dbf. */
    using Constructor = LRaceCategoryTable*(__thiscall*)(LRaceCategoryTable* thisptr,
                                                         const char* globalsFolderPath,
                                                         void* codeBaseEnvProxy);
    Constructor constructor;

    /** Performs initialization before reading categories. */
    using Init = void(__stdcall*)(LRaceCategoryTable* table,
                                  void* codeBaseEnvProxy,
                                  const char* globalsFolderPath,
                                  const char* dbfFileName);
    Init init;

    /**
     * Reads race category from dbf file using category name.
     * Throws exception if category with specified name could not be found in dbf table.
     * @param[inout] category race category structure to store the result.
     * @param[in] table initialized category table to search for RaceId constants by name.
     * @param[in] fieldName dbf table field name that represents category.
     * @param[in] dbfFileName name of dbf table to read from.
     * @returns pointer to category.
     */
    using ReadCategory = LRaceCategory*(__stdcall*)(LRaceCategory* category,
                                                    LRaceCategoryTable* table,
                                                    const char* fieldName,
                                                    const char* dbfFileName);
    ReadCategory readCategory;

    /** Performs necessary cleanup after categories was read. */
    using InitDone = void*(__thiscall*)(LRaceCategoryTable* table);
    InitDone initDone;
};

Api& get();

/** Returns address of LRaceCategoryTable::vftable used in game. */
const void* vftable();

} // namespace LRaceCategoryTableApi

} // namespace game

#endif // RACECATEGORY_H
