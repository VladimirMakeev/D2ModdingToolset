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

#ifndef BUILDINGTYPE_H
#define BUILDINGTYPE_H

#include "buildingcat.h"
#include "currency.h"
#include "midgardid.h"
#include "midobject.h"
#include "mqpoint.h"
#include "textandid.h"
#include "unitbranchcat.h"

namespace game {

struct GlobalData;
struct CDBTable;

struct TBuildingTypeData
{
    int frame1;
    int frame2;
    CMqPoint pos;
    CMqPoint pos2;
    int posZ;
    TextAndId name;
    TextAndId description;
    char picture[9];
    char padding[3];
    CMidgardID requiredId;
    Bank cost;
    LBuildingCategory category;
};

assert_size(TBuildingTypeData, 84);

/** Holds building information read from Gbuild.dbf. */
struct TBuildingType : public game::IMidObject
{
    TBuildingTypeData* data;
};

assert_size(TBuildingType, 12);

/** Holds unit upgrade building information read from Gbuild.dbf. */
struct TBuildingUnitUpgType : public TBuildingType
{
    LUnitBranch branch;
    int level;
};

assert_size(TBuildingUnitUpgType, 28);

namespace TBuildingTypeApi {

struct Api
{
    using Constructor = TBuildingType*(__thiscall*)(TBuildingType* thisptr,
                                                    const CDBTable* dbTable,
                                                    const GlobalData** globalData);
    Constructor constructor;
};

Api& get();

} // namespace TBuildingTypeApi

namespace TBuildingUnitUpgTypeApi {

const void* vftable();

}

} // namespace game

#endif // BUILDINGTYPE_H
