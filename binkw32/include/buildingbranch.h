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

#ifndef BUILDINGBRANCH_H
#define BUILDINGBRANCH_H

#include "categories.h"

namespace game {

struct TBuildingUnitUpgType;
struct TBuildingType;

struct CBuildingBranchData
{
    int unk1;
    int unk2;
    int unk3;
    int unk4;
    int unk5;
    int unk6;
    int unk7;
    int branchNumber;
    LRaceCategory raceCategory;
    int unk12;
    int unk13;
    int unk14;
    int unk15;
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

    using InitData2 = void*(__thiscall*)(void*);
    InitData2 initData2;

    using InitData3 = void*(__thiscall*)(void*);
    InitData3 initData3;

    /**
     * Adds sideshow unit building to building branch.
     * @param[in] unknown pointer to CBuildingBranch::data->unk1.
     * @param[in] building sideshow unit building to add.
     */
    using AddSideshowUnitBuilding = void(__stdcall*)(int* unknown,
                                                     const TBuildingUnitUpgType* building);
    AddSideshowUnitBuilding addSideshowUnitBuilding;

    /**
     * Adds unit building to building branch.
     * @param[in] phaseGame
     * @param[in] data pointer to building branch data.
     * @param[in] building unit building to add.
     */
    using AddUnitBuilding = void(__stdcall*)(int phaseGame,
                                             const CBuildingBranchData* data,
                                             const TBuildingUnitUpgType* building);
    AddUnitBuilding addUnitBuilding;

    /**
     * Adds building to building branch.
     * @param[in] phaseGame
     * @param[in] data pointer to building branch data.
     * @param[in] building building to add.
     */
    using AddBuilding = void(__stdcall*)(int phaseGame,
                                         const CBuildingBranchData* data,
                                         const TBuildingType* building);
    AddBuilding addBuilding;
};

Api& get();

const void* vftable();

} // namespace CBuildingBranchApi

} // namespace game

#endif // BUILDINGBRANCH_H
