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

#ifndef BUILDINGBRANCH_H
#define BUILDINGBRANCH_H

#include "d2map.h"
#include "d2string.h"
#include "racecategory.h"

namespace game {

struct TBuildingUnitUpgType;
struct TBuildingType;
struct CPhaseGame;

/**
 * Each node stores pair of building id type indices.
 * First element in pair is a id type index of TBuildingType::buildingId.
 * Second element in pair is a id type index of TBuildingTypeData::requiredId.
 */
using BuildingBranchMap = Map<int, int>;

struct CBuildingBranchData
{
    BuildingBranchMap map;
    int branchNumber;
    LRaceCategory raceCategory;
    String branchDialogName;
};

static_assert(sizeof(CBuildingBranchData) == 60,
              "Size of CBuildingBranchData structure must be exactly 60 bytes");

struct CBuildingBranch
{
    const void* vftable;
    CBuildingBranchData* data;
};

namespace CBuildingBranchApi {

struct Api
{
    /** Creates CBuildingBranch for specified branchNumber. */
    using Constructor = CBuildingBranch*(__thiscall*)(CBuildingBranch* thisptr,
                                                      int phaseGame,
                                                      int* branchNumber);
    Constructor constructor;

    /** Initializes CBuildingBranchData with default values. */
    using InitData = CBuildingBranchData*(__thiscall*)(CBuildingBranchData* thisptr);
    InitData initData;

    using InitBranchMap = void*(__thiscall*)(BuildingBranchMap* map);
    InitBranchMap initBranchMap;

    /**
     * Adds sideshow unit building to building branch.
     * @param[in] unknown pointer to CBuildingBranch::data->unk1.
     * @param[in] building sideshow unit building to add.
     */
    using AddSideshowUnitBuilding = void(__stdcall*)(BuildingBranchMap* map,
                                                     const TBuildingUnitUpgType* building);
    AddSideshowUnitBuilding addSideshowUnitBuilding;

    /**
     * Adds unit building to building branch.
     * @param[in] phaseGame
     * @param[in] data pointer to building branch data.
     * @param[in] building unit building to add.
     */
    using AddUnitBuilding = void(__stdcall*)(CPhaseGame* phaseGame,
                                             BuildingBranchMap* map,
                                             const TBuildingUnitUpgType* building);
    AddUnitBuilding addUnitBuilding;

    /**
     * Adds building to building branch.
     * @param[in] phaseGame
     * @param[in] data pointer to building branch data.
     * @param[in] building building to add.
     */
    using AddBuilding = void(__stdcall*)(CPhaseGame* phaseGame,
                                         BuildingBranchMap* map,
                                         const TBuildingType* building);
    AddBuilding addBuilding;

    using CreateDialogName = char*(__thiscall*)(CBuildingBranch* thisptr);
    CreateDialogName createDialogName;
};

Api& get();

const void* vftable();

} // namespace CBuildingBranchApi

} // namespace game

#endif // BUILDINGBRANCH_H
