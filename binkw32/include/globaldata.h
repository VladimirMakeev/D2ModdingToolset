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

#ifndef GLOBALDATA_H
#define GLOBALDATA_H

namespace game {

struct LAttackClassTable;
struct LAttackSourceTable;
struct LAttackReachTable;
struct LUnitBranchTable;
struct LUnitCategoryTable;
struct LBuildingCategoryTable;
struct LGroundCategoryTable;
struct LDifficultyLevelTable;
struct LItemCategoryTable;
struct LLordCategoryTable;
struct LModifierElementTable;
struct LOrderCategoryTable;
struct LRaceCategoryTable;
struct LSubRaceCategoryTable;
struct LResourceTypeTable;
struct LSiteCategoryTable;
struct LSpellCategoryTable;
struct LTerrainCategoryTable;
struct LLeaderAbilityTable;
struct LLeaderCategoryTable;
struct LAiSpellCategoryTable;
struct LModifGroupTable;
struct LAttitudesCategoryTable;
struct LAiMsgCategoryTable;
struct LNobleActionCatTable;
struct LImmuneCatTable;
struct LFortCategoryTable;
struct LEventCondCategoryTable;
struct LEventEffectCategoryTable;
struct LLandmarkCategoryTable;
struct LDeathAnimCategoryTable;

struct CMidgardID;

/** Holds global game information. */
struct GlobalData
{
    void* unknown;
    LAttackClassTable* attackClasses;
    LAttackSourceTable* attackSources;
    LAttackReachTable* attackReach;
    LUnitBranchTable* unitBranches;
    LUnitCategoryTable* unitCategories;
    LBuildingCategoryTable* buildingCategories;
    LGroundCategoryTable* groundCategories;
    LDifficultyLevelTable* difficultyLevels;
    LItemCategoryTable* itemCategories;
    LLordCategoryTable* lordCategories;
    LModifierElementTable* modifierElements;
    LOrderCategoryTable* orderCategories;
    LRaceCategoryTable* raceCategories;
    LSubRaceCategoryTable* subRaceCategories;
    LResourceTypeTable* resourceTypes;
    LSiteCategoryTable* siteCategories;
    LSpellCategoryTable* spellCategories;
    LTerrainCategoryTable* terrainCategories;
    LLeaderAbilityTable* leaderAbilities;
    LLeaderCategoryTable* leaderCategories;
    LAiSpellCategoryTable* aiSpellCategories;
    LModifGroupTable* modifGroups;
    LAttitudesCategoryTable* attitudesCategories;
    LAiMsgCategoryTable* aiMsgCategories;
    LNobleActionCatTable* nobleActionCategories;
    LImmuneCatTable* immuneCategories;
    LFortCategoryTable* fortCategories;
    LEventCondCategoryTable* eventCondCategories;
    LEventEffectCategoryTable* eventEffectCategories;
    LLandmarkCategoryTable* landmarkCategories;
    LDeathAnimCategoryTable* deathAnimCategories;
    int* buildings;
    int* races;
    int* subRaces;
    int* lords;
    int* spells;
    int* units;
    int* modifiers;
    int* attacks;
    int* landmarks;
    int* itemTypes;
    int* actions;
    int* transf;
    int* dynUpgrade;
    int* tileVariation;
    int* aiAttitudes;
    int* aiMessages;
    int* globalVariables;
    int* unitGenerator;
    int initialized;
};

static_assert(sizeof(GlobalData) == 204, "Size of GlobalData structure must be exactly 204 bytes");

namespace GlobalDataApi {

struct Api
{
    using GetGlobalData = GlobalData**(__cdecl*)();
    GetGlobalData getGlobalData;

    /** Searches an entity by its id. */
    using FindById = void*(__thiscall*)(void* entityCollection, const CMidgardID* id);
    FindById findById;
};

Api& get();

} // namespace GlobalDataApi

} // namespace game

#endif // GLOBALDATA_H
