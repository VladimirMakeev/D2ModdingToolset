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

#ifndef UMSTACK_H
#define UMSTACK_H

#include "groundcat.h"
#include "leaderabilitycat.h"
#include "smartptr.h"
#include "textandid.h"
#include "ummodifier.h"
#include "usstackleader.h"
#include "usunit.h"

namespace game {

struct CUmStackData;

struct CUmStack
{
    IUsUnit usUnit;
    IUsStackLeader usStackLeader;
    CUmModifier umModifier;
    CUmStackData* data;
};

assert_size(CUmStack, 24);

struct CUmStackData
{
    ModifierValue moveAllowance;
    ModifierValue scoutingRange;
    ModifierValue leadership;
    SmartPtr<LGroundCategory> moveAbility;
    SmartPtr<LLeaderAbility> leaderAbility;
    TextAndId desc;
    bool fastRetreat;
    char padding[3];
    ModifierValue lowerCost;
};

assert_size(CUmStackData, 60);

namespace CUmStackApi {

struct Vftable
{
    const IUsUnitVftable* usUnit;
    const IUsStackLeaderVftable* usStackLeader;
    const CUmModifierVftable* umModifier;
};

Vftable& vftable();

} // namespace CUmStackApi

} // namespace game

#endif // UMSTACK_H
