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

#ifndef UNITSFORHIRE_H
#define UNITSFORHIRE_H

#include <filesystem>
#include <vector>

namespace game {
struct CMidgardID;
}; // namespace game

namespace hooks {

using IdsArray = std::vector<game::CMidgardID>;

/**
 * Each race has an array of units for hire.
 * Race indices are RaceId values.
 * Neutrals has empty array.
 */
using UnitsForHire = std::vector<IdsArray>;

/**
 * Tries to load new units for hire in cities.
 * Searches Grace.dbf for new columns 'SOLDIER_N', starting from N = 6.
 * New units can be accessed using unitsForHire().
 * @returns false in case of database access error.
 */
bool loadUnitsForHire();

const UnitsForHire& unitsForHire();

} // namespace hooks

#endif // UNITSFORHIRE_H
