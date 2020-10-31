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

#ifndef HOOKS_H
#define HOOKS_H

#include "idlist.h"
#include <string>

namespace game {
struct AutoDialogData;
struct CMenuNewSkirmishSingle;
struct CMidDataCache2;
struct CMidgardID;
struct GlobalData;
struct CDBTable;
struct LBuildingCategoryTable;
struct CBuildingBranch;
struct IUsSoldier;
}; // namespace game

namespace hooks {

void respopupInitHooked(void);
void* __fastcall toggleShowBannersInitHooked(void* thisptr, int /*%edx*/);

bool __fastcall processUnitModifiersHooked(void* thisptr, int /*%edx*/, int* a2);

game::AutoDialogData* __fastcall loadScriptFileHooked(game::AutoDialogData* thisptr,
                                                      int /*%edx*/,
                                                      const char* filePath,
                                                      int /*unknown*/);

game::CMenuNewSkirmishSingle* __fastcall menuNewSkirmishSingleCtorHooked(
    game::CMenuNewSkirmishSingle* thisptr,
    int /*%edx*/,
    int a1);

bool __stdcall addPlayerUnitsToHireListHooked(game::CMidDataCache2* dataCache,
                                              const game::CMidgardID* playerId,
                                              const game::CMidgardID* a3,
                                              game::IdList* hireList);

/**
 * Create TBuildingUnitUpgType objects for buildings from heal category.
 * This allows to assign temples as buildings required to hire units.
 */
void __stdcall createBuildingTypeHooked(const game::CDBTable* dbTable,
                                        void* a2,
                                        const game::GlobalData** globalData);

game::LBuildingCategoryTable* __fastcall buildingCategoryTableCtorHooked(
    game::LBuildingCategoryTable* thisptr,
    int /*%edx*/,
    const char* globalsFolderPath,
    void* codeBaseEnvProxy);

/** Shows native in-game message box with specified text. */
void showMessageBox(const std::string& message);

game::CBuildingBranch* __fastcall buildingBranchCtorHooked(game::CBuildingBranch* thisptr,
                                                           int /*%edx*/,
                                                           int phaseGame,
                                                           int* branchNumber);

int __stdcall chooseUnitLaneHooked(const game::IUsSoldier* soldier);

} // namespace hooks

#endif // HOOKS_H
