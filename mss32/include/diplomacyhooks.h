/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#ifndef DIPLOMACYHOOKS_H
#define DIPLOMACYHOOKS_H

#include <cstdint>

namespace game {
struct CMidDiplomacy;
struct LRaceCategory;
}

namespace hooks {

std::uint32_t* __fastcall getCurrentRelationHooked(const game::CMidDiplomacy* thisptr,
                                                   int /*%edx*/,
                                                   std::uint32_t* current,
                                                   const game::LRaceCategory* race1,
                                                   const game::LRaceCategory* race2);

std::uint32_t* __fastcall getPreviousRelationHooked(const game::CMidDiplomacy* thisptr,
                                                    int /*%edx*/,
                                                    std::uint32_t* previous,
                                                    const game::LRaceCategory* race1,
                                                    const game::LRaceCategory* race2);

bool __fastcall areAlliesHooked(const game::CMidDiplomacy* thisptr,
                                int /*%edx*/,
                                const game::LRaceCategory* race1,
                                const game::LRaceCategory* race2);


std::uint32_t* __fastcall getAllianceTurnHooked(const game::CMidDiplomacy* thisptr,
                                                int /*%edx*/,
                                                std::uint32_t* turn,
                                                const game::LRaceCategory* race1,
                                                const game::LRaceCategory* race2);

bool __fastcall areAlwaysAtWarHooked(const game::CMidDiplomacy* thisptr,
                                     int /*%edx*/,
                                     const game::LRaceCategory* race1,
                                     const game::LRaceCategory* race2);

bool __fastcall aiCouldNotBreakAllianceHooked(const game::CMidDiplomacy* thisptr,
                                              int /*%edx*/,
                                              const game::LRaceCategory* race1,
                                              const game::LRaceCategory* race2);

bool __stdcall areRelationsAtWarHooked(const std::uint32_t* relation);
bool __stdcall areRelationsNeutralHooked(const std::uint32_t* relation);
bool __stdcall areRelationsPeacefulHooked(const std::uint32_t* relation);

}

#endif // DIPLOMACYHOOKS_H