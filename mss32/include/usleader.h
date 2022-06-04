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

#ifndef USLEADER_H
#define USLEADER_H

#include "usunitextension.h"

namespace game {

struct Bank;
struct CMidgardID;
struct LLeaderCategory;
struct GlobalData;
struct IUsLeaderVftable;

/** Leader interface. */
struct IUsLeader : public IUsUnitExtensionT<IUsLeaderVftable>
{ };

struct IUsLeaderVftable : public IUsUnitExtensionVftable
{
    using GetCategory = const LLeaderCategory*(__thiscall*)(const IUsLeader* thisptr);
    GetCategory getCategory;

    using GetPrevUnitImplId = const CMidgardID*(__thiscall*)(const IUsLeader* thisptr);
    GetPrevUnitImplId getPrevUnitImplId;

    using GetEnrollCost = const Bank*(__thiscall*)(const IUsLeader* thisptr);
    GetEnrollCost getEnrollCost;

    /** Checks that leader data is correct. */
    using Link = void(__thiscall*)(const IUsLeader* thisptr, const GlobalData** globalData);
    Link link;
};

assert_vftable_size(IUsLeaderVftable, 5);

} // namespace game

#endif // USLEADER_H
