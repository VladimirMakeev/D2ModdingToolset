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

#ifndef UMUNITHOOKS_H
#define UMUNITHOOKS_H

namespace game {
struct CMidgardID;
struct GlobalData;
struct CDBTable;
struct IAttack;
struct IUsSoldier;
struct CUmUnit;
} // namespace game

namespace hooks {

game::CUmUnit* __fastcall umUnitCtorHooked(game::CUmUnit* thisptr,
                                           int /*%edx*/,
                                           const game::CMidgardID* modifierId,
                                           game::CDBTable* dbTable,
                                           const game::GlobalData** globalData);

game::CUmUnit* __fastcall umUnitCopyCtorHooked(game::CUmUnit* thisptr,
                                               int /*%edx*/,
                                               const game::CUmUnit* src);

int* __fastcall umUnitGetRegenHooked(const game::IUsSoldier* thisptr, int /*%edx*/);

} // namespace hooks

#endif // UMUNITHOOKS_H
