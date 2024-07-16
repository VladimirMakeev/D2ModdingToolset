/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef TESTSTACKEXISTSHOOKS_H
#define TESTSTACKEXISTSHOOKS_H

namespace game {
struct CTestStackExists;
struct IMidgardObjectMap;
struct CMidgardID;
} // namespace game

namespace hooks {

bool __fastcall testStackExistsHooked(const game::CTestStackExists* thisptr,
                                      int /*%edx*/,
                                      const game::IMidgardObjectMap* objectMap,
                                      const game::CMidgardID* playerId,
                                      const game::CMidgardID* eventId);

}

#endif // TESTSTACKEXISTSHOOKS_H
