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

#ifndef RESETSTACKEXT_H
#define RESETSTACKEXT_H

#include "idvector.h"

namespace game {

struct IResetStackExtVftable;

struct IResetStackExt
{
    IResetStackExtVftable* vftable;
};

struct IResetStackExtVftable
{
    using Destructor = void(__thiscall*)(IResetStackExt* thisptr, bool freeMemory);
    Destructor destructor;

    using HireLeader = bool(
        __thiscall*)(IResetStackExt* thisptr, const CMidgardID* unitImplId, int a3, int a4, int a5);
    HireLeader hireLeader;

    void* methods[2];

    /**
     * Returns ids of units or leaders that can be hired.
     * @param[in] thisptr object pointer.
     * @param[inout] unitImplIds vector where ids will be stored.
     * @param leaders - if set to 1, leader ids will be returned.
     * Any other value results in return of unit ids. Game logic uses 2 for units here.
     */
    using GetUnitIdsForHire = void(__thiscall*)(IResetStackExt* thisptr,
                                                IdVector* unitImplIds,
                                                int leaders);
    GetUnitIdsForHire getUnitIdsForHire;

    using GetStackId = CMidgardID*(__thiscall*)(IResetStackExt* thisptr, CMidgardID* value);
    GetStackId getStackId;
};

assert_vftable_size(IResetStackExtVftable, 6);

} // namespace game

#endif // RESETSTACKEXT_H
