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

#ifndef USSTACKLEADERIMPL_H
#define USSTACKLEADERIMPL_H

#include "d2map.h"
#include "groundcat.h"
#include "leaderabilitycat.h"
#include "midgardid.h"
#include "textandid.h"
#include "usstackleader.h"

namespace game {

/** Holds leader specific data read from GUnits.dbf. */
struct TUsStackLeaderImplData
{
    CMidgardID unitId; /**< UNIT_ID */
    int movement;      /**< MOVE */
    int scout;         /**< SCOUT */
    int leadership;    /**< LEADERSHIP */
    int negotiate;     /**< NEGOTIATE */
    bool unknown;
    char padding[3];
    TextAndId abilityText; /**< ABIL_TXT */
    Map<LGroundCategory> moveBonuses;
    Map<LLeaderAbility> abililies;
};

static_assert(sizeof(TUsStackLeaderImplData) == 72,
              "Size of TUsStackLeaderImplData structure must be exactly 72 bytes");

/** Implementation of stack leader interface. */
struct TUsStackLeaderImpl : public IUsStackLeader
{
    TUsStackLeaderImplData* data;
};

} // namespace game

#endif // USSTACKLEADERIMPL_H
