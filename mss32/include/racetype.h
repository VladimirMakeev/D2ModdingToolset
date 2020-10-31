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

#ifndef RACETYPE_H
#define RACETYPE_H

#include "categories.h"
#include "currency.h"
#include "d2vector.h"
#include "midgardid.h"
#include "midobject.h"
#include "textandid.h"

namespace game {

/**
 * Holds leader names read from TLeader.dbf.
 * Must be 24 bytes according to TRaceType constructor.
 */
struct CRaceLeaderNames
{
    char unknown[24];
};

/**
 * Holds player descriptions read from TPlayer.dbf.
 * Must be 18 bytes according to TRaceType constructor.
 */
struct CRacePlayerDesc
{
    char unknown[18];
};

/** Holds city information read from GCityInf.dbf. */
struct TRaceTypeCityInformation;

using IdVector = Vector<CMidgardID>;

struct TRaceTypeData
{
    LRaceCategory raceType;
    TextAndId name;
    bool playable;
    char padding[3];
    CMidgardID guardianUnitId;
    CMidgardID nobleUnitId;
    IdVector leaders;
    IdVector soldiers;
    int regen;
    int scout;
    Bank income;
    CMidgardID protectionModifierId;
    CRaceLeaderNames* leaderNames;
    CRacePlayerDesc* playerDesc1;
    CRacePlayerDesc* playerDesc2;
    TRaceTypeCityInformation* cityInfo;
};

static_assert(sizeof(TRaceTypeData) == 104,
              "Size of TRaceTypeData structure must be exactly 104 bytes");

/** Holds race information read from Grace.dbf. */
struct TRaceType : public IMidObject
{
    CMidgardID raceId;
    TRaceTypeData* data;
};

static_assert(sizeof(TRaceType) == 12, "Size of TRaceType structure must be exactly 12 bytes");

} // namespace game

#endif // RACETYPE_H
