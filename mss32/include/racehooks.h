/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#ifndef NEWRACEHOOKS_H
#define NEWRACEHOOKS_H

#include "globaldata.h"

namespace game {
struct LRaceCategoryTable;
}

namespace hooks {

/** Reads new race categories from LRace.dbf. */
game::LRaceCategoryTable* __fastcall raceCategoryTableCtorHooked(game::LRaceCategoryTable* thisptr,
                                                                 int /*%edx*/,
                                                                 const char* globalsFolderPath,
                                                                 void* codeBaseEnvProxy);

void __fastcall validateRacesHooked(game::RacesMap** thisptr,
                                    int /*%edx*/,
                                    game::GlobalData** globalData);

} // namespace hooks

#endif // NEWRACEHOOKS_H
