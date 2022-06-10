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

#ifndef USSOLDIERIMPL_H
#define USSOLDIERIMPL_H

#include "currency.h"
#include "d2pair.h"
#include "deathanimcat.h"
#include "midgardid.h"
#include "mq_c_s.h"
#include "subracecat.h"
#include "textandid.h"
#include "unitbranchcat.h"
#include "ussoldier.h"
#include <cstddef>

namespace game {

using AttackSourceImmuneMap = mq_c_s<Pair<LAttackSource, LImmuneCat>>;
using AttackClassImmuneMap = mq_c_s<Pair<LAttackClass, LImmuneCat>>;

struct TUsSoldierImplData
{
    CMidgardID unitId;
    TextAndId name;
    TextAndId description;
    int level;
    int hitPoints;
    int armor;
    CMidgardID baseUnitImplId;
    LDeathAnimCategory deathAnimation;
    int xpNext;
    int xpKilled;
    int regen;
    Bank enrollCost;
    Bank reviveCost;
    Bank healCost;
    Bank trainingCost;
    CMidgardID raceId;
    LSubRaceCategory subrace;
    CMidgardID attackId;
    CMidgardID attack2Id;
    LUnitBranch branch;
    AttackSourceImmuneMap attackSourceImmuneMap;
    AttackClassImmuneMap attackClassImmuneMap;
    bool sizeSmall;
    bool sexM;
    bool attackTwice;
    char padding;
    int dynUpg1;
    int dynUpgLvl;
    int dynUpg2;
    bool waterOnly;
    char padding2[3];
};

assert_size(TUsSoldierImplData, 204);
assert_offset(TUsSoldierImplData, sizeSmall, 184);

/** Base class for all unit types, holds data read from GUnits.dbf. */
struct TUsSoldierImpl : public IUsSoldier
{
    TUsSoldierImplData* data;
};

namespace TUsSoldierImplApi {

const IUsSoldierVftable* vftable();

} // namespace TUsSoldierImplApi

} // namespace game

#endif // USSOLDIERIMPL_H
