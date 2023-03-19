/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#ifndef AISTACK_H
#define AISTACK_H

#include "d2assert.h"
#include "idlist.h"

namespace game {

struct CMqPoint;
struct CMidPlayer;
struct CMidSubRace;
struct CMidStack;
struct CAiAttitudes;
struct IAiStackVftable;

struct IAiStack
{
    IAiStackVftable* vftable;
};

struct IAiStackVftable
{
    using Destructor = void(__thiscall*)(IAiStack* thisptr, char flags);
    Destructor destructor;

    using GetId = const CMidgardID*(__thiscall*)(const IAiStack* thisptr);
    GetId getId;

    using GetStack = const CMidStack*(__thiscall*)(const IAiStack* thisptr);
    GetStack getStack;

    using GetPosition = const CMqPoint*(__thiscall*)(const IAiStack* thisptr, CMqPoint* value);
    GetPosition getPosition;

    using GetBool = bool(__thiscall*)(const IAiStack* thisptr);
    GetBool isInside;
    GetBool isWaterOnly;
    GetBool isOwnerCaptured;
    GetBool hasDeadOrLowHpUnits;
    GetBool inCityUnderRiot;
    GetBool getUnknown;

    using SetBool = void(__thiscall*)(IAiStack* thisptr, bool value);
    SetBool setUnknown;

    using IsAdjacentTo = bool(__thiscall*)(const IAiStack* thisptr, const CMqPoint* position);
    IsAdjacentTo isAdjacentTo;

    /** Simply calls getDistance2 and converts result to double. */
    using GetDistance = double(__thiscall*)(const IAiStack* thisptr,
                                            const game::CMqPoint* position);
    GetDistance getDistance;

    using GetDistance2 = int(__thiscall*)(const IAiStack* thisptr, const game::CMqPoint* position);
    GetDistance2 getDistance2;

    using GetOwner = const CMidPlayer*(__thiscall*)(const IAiStack* thisptr);
    GetOwner getOwner;

    using GetSubRace = const CMidSubRace*(__thiscall*)(const IAiStack* thisptr);
    GetSubRace getSubRace;

    using GetAttitudes = CAiAttitudes*(__thiscall*)(const IAiStack* thisptr);
    GetAttitudes getAttitudes;

    void* methods[2];

    /** Involves internal IAiActionFilter. */
    using UnknownMethod = void(__thiscall*)(const IAiStack* thisptr, IdList* list, int a3);
    UnknownMethod method19;
    UnknownMethod method20;
};

assert_vftable_size(IAiStackVftable, 21);

} // namespace game

#endif // AISTACK_H
