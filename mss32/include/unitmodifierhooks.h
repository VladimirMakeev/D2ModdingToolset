/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#ifndef UNITMODIFIERHOOKS_H
#define UNITMODIFIERHOOKS_H

namespace game {
struct GlobalData;
struct CDBTable;
struct TUnitModifier;
} // namespace game

namespace hooks {

game::TUnitModifier* __fastcall unitModifierCtorHooked(game::TUnitModifier* thisptr,
                                                       int /*%edx*/,
                                                       const game::CDBTable* dbTable,
                                                       const char* globalsFolderPath,
                                                       void* codeBaseEnvProxy,
                                                       const game::GlobalData** globalData);

void __fastcall unitModifierDtorHooked(game::TUnitModifier* thisptr, int /*%edx*/, char flags);

} // namespace hooks

#endif // UNITMODIFIERHOOKS_H
