/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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
#include "d2map.h"
#include "d2pair.h"
#include "deathanimcat.h"
#include "midgardid.h"
#include "subracecat.h"
#include "textandid.h"
#include "unitbranchcat.h"
#include "ussoldier.h"
#include <cstddef>

namespace game {

using AttackSourceImmuneMap = Map<Pair<LAttackSource, LImmuneCat>>;
using AttackClassImmuneMap = Map<Pair<LAttackClass, LImmuneCat>>;

struct TUsSoldierImplData
{
    CMidgardID unitId;
    TextAndId name;
    TextAndId description;
    int level;
    int hitPoints;
    int armor;
    CMidgardID baseUnitId;
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

static_assert(sizeof(TUsSoldierImplData) == 204,
              "Size of TUsSoldierImplData structure must be exactly 204 bytes");

static_assert(offsetof(TUsSoldierImplData, sizeSmall) == 184,
              "TUsSoldierImplData::sizeSmall offset must be 184 bytes");

/** Base class for all unit types, holds data read from GUnits.dbf. */
struct TUsSoldierImpl : public IUsSoldier
{
    TUsSoldierImplData* data;
};

} // namespace game

#endif // USSOLDIERIMPL_H
