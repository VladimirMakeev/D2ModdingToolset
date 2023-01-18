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

#ifndef MENUPHASEHOOKS_H
#define MENUPHASEHOOKS_H

namespace game {
struct CMenuPhase;

}

namespace hooks {

game::CMenuPhase* __fastcall menuPhaseCtorHooked(game::CMenuPhase* thisptr,
                                                 int /*%edx*/,
                                                 int a2,
                                                 int a3);

void __fastcall menuPhaseDtorHooked(game::CMenuPhase* thisptr, int /*%edx*/, char flags);

void __fastcall menuPhaseSetTransitionHooked(game::CMenuPhase* thisptr,
                                             int /*%edx*/,
                                             int transition);

} // namespace hooks

#endif // MENUPHASEHOOKS_H
