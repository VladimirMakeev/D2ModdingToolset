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

#include "groundcat.h"
#include "leaderabilitycat.h"
#include "midgardid.h"
#include "mq_c_s.h"
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
    bool fastRetreat;
    char padding[3];
    TextAndId abilityText; /**< ABIL_TXT */
    mq_c_s<LGroundCategory> moveBonuses;
    mq_c_s<LLeaderAbility> abililies;
};

assert_size(TUsStackLeaderImplData, 72);

struct TUsStackLeaderImplVftable : public IUsStackLeaderVftable
{
    /** Checks that stack leader data is correct. */
    using Link = void(__thiscall*)(const IUsStackLeader* thisptr, const GlobalData** globalData);
    Link link;
};

assert_vftable_size(TUsStackLeaderImplVftable, 11);

/** Implementation of stack leader interface. */
struct TUsStackLeaderImpl : public IUsStackLeaderT<TUsStackLeaderImplVftable>
{
    TUsStackLeaderImplData* data;
};

} // namespace game

#endif // USSTACKLEADERIMPL_H
