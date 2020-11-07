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

#ifndef MIDVILLAGE_H
#define MIDVILLAGE_H

#include "d2vector.h"
#include "fortification.h"
#include "racecategory.h"

namespace game {

/** Named as mq_c_s in game code. */
struct CMidVillageRaceData
{
    const void* vftable;
    Vector<LRaceCategory> raceCategories;
};

/** Holds city related data in scenario file and game. */
struct CMidVillage : public CFortification
{
    int tierLevel;
    int growthTurn;
    int riotTurn;
    int regeneration;
    CMidgardID protectionId;
    CMidVillageRaceData raceData;
};

static_assert(sizeof(CMidVillage) == 184,
              "Size of CMidVillage structure must be exactly 184 bytes");

static_assert(offsetof(CMidVillage, tierLevel) == 144,
              "CMidVillage::tierLevel offset must be 144 bytes");

static_assert(offsetof(CMidVillage, raceData.raceCategories) == 168,
              "CMidVillage::raceData.raceCategories offset must be 168 bytes");

} // namespace game

#endif // MIDVILLAGE_H
