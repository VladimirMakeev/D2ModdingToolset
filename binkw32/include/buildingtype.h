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

#ifndef BUILDINGTYPE_H
#define BUILDINGTYPE_H

#include "buildingcat.h"
#include "currency.h"
#include "midgardid.h"
#include "midobject.h"
#include "textandid.h"
#include "unitbranchcat.h"

namespace game {

struct GlobalData;

struct Position
{
    int x;
    int y;
};

struct TBuildingTypeData
{
    int frame1;
    int frame2;
    Position pos;
    Position pos2;
    int posZ;
    TextAndId name;
    TextAndId description;
    char picture[9];
    char padding[3];
    CMidgardID requiredId;
    Bank cost;
    LBuildingCategory category;
};

static_assert(sizeof(TBuildingTypeData) == 84,
              "Size of TBuildingTypeData structure must be exactly 84 bytes");

/** Holds building information read from Gbuild.dbf. */
struct TBuildingType : public game::IMidObject
{
    CMidgardID buildingId;
    TBuildingTypeData* data;
};

static_assert(sizeof(TBuildingType) == 12,
              "Size of TBuildingType structure must be exactly 12 bytes");

/** Holds unit upgrade building information read from Gbuild.dbf. */
struct TBuildingUnitUpgType : public TBuildingType
{
    LUnitBranch branch;
    int level;
};

static_assert(sizeof(TBuildingUnitUpgType) == 28,
              "Size of TBuildingUnitUpgType structure must be exactly 28 bytes");

namespace TBuildingTypeApi {

struct Api
{
    using Constructor = TBuildingType*(__thiscall*)(TBuildingType* thisptr,
                                                    void* dbTable,
                                                    GlobalData** globalData);
    Constructor constructor;
};

Api& get();

} // namespace TBuildingTypeApi

} // namespace game

#endif // BUILDINGTYPE_H
