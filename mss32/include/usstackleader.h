/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef USSTACKLEADER_H
#define USSTACKLEADER_H

#include "usunitextension.h"

namespace game {

struct LGroundCategory;
struct LLeaderAbility;
struct GlobalData;
struct IUsStackLeaderVftable;

/** Stack leader interface. */
struct IUsStackLeader : public IUsUnitExtensionT<IUsStackLeaderVftable>
{ };

struct IUsStackLeaderVftable : public IUsUnitExtensionVftable
{
    using GetInt = int(__thiscall*)(const IUsStackLeader* thisptr);

    /** Returns maximum movement points stack leader can have. */
    GetInt getMovement;

    /** Returns leader ability name. */
    using GetAbilityName = const char*(__thiscall*)(const IUsStackLeader* thisptr);
    GetAbilityName getAbilityName;

    /** Returns true if leader has movement bonus on specified ground. */
    using HasMovementBonus = bool(__thiscall*)(const IUsStackLeader* thisptr,
                                               const LGroundCategory* ground);
    HasMovementBonus hasMovementBonus;

    /** Returns scouting range. */
    GetInt getScout;

    GetInt getLeadership;

    GetInt getNegotiate;

    /** Returns true if leader has specified ability. */
    using HasAbility = bool(__thiscall*)(const IUsStackLeader* thisptr,
                                         const LLeaderAbility* ability);
    HasAbility hasAbility;

    using GetUnknown = bool(__thiscall*)(const IUsStackLeader* thisptr);
    GetUnknown getUnknown;

    GetInt getLowerCost;

    /** Checks that stack leader data is correct. */
    using Link = void(__thiscall*)(const IUsStackLeader* thisptr, const GlobalData** globalData);
    Link link;
};

static_assert(sizeof(IUsStackLeaderVftable) == 11 * sizeof(void*),
              "IUsStackLeader vftable must have exactly 11 methods");

} // namespace game

#endif // USSTACKLEADER_H
