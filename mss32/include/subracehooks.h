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

#ifndef SUBRACEHOOKS_H
#define SUBRACEHOOKS_H

#include "subracecat.h"
#include <string>
#include <vector>

namespace hooks {

struct SubRaceDescription
{
    game::LSubRaceCategory category{};
    std::string categoryName;
};

using NewSubRaces = std::vector<SubRaceDescription>;

/** Returns new sub races categories and category names, if any. */
NewSubRaces& newSubRaces();

/** Reads new subrace categories from LSubRace.dbf. */
game::LSubRaceCategoryTable* __fastcall subRaceCategoryTableCtorHooked(
    game::LSubRaceCategoryTable* thisptr,
    int /*%edx*/,
    const char* globalsFolderPath,
    void* codeBaseEnvProxy);

} // namespace hooks

#endif // SUBRACEHOOKS_H
