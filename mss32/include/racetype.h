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

#ifndef RACETYPE_H
#define RACETYPE_H

#include "currency.h"
#include "idvector.h"
#include "midgardid.h"
#include "midobject.h"
#include "racecategory.h"
#include "textandid.h"

namespace game {

struct GlobalData;

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

struct TRaceTypeVftable : public IMidObjectVftable
{
    /**
     * Validates contents of TRaceType object.
     * Throws exception if race has invalid units, leaders or guardian.
     * Also checks for validity of unit branches and city protection modifiers.
     */
    using Validate = void(__thiscall*)(TRaceType* thisptr, GlobalData** globalData);
    Validate validate;
};

static_assert(sizeof(TRaceTypeVftable) == 2 * sizeof(void*),
              "TRaceType vftable must have exactly 2 methods");

} // namespace game

#endif // RACETYPE_H
