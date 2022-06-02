/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef ENCLAYOUTUNIT_H
#define ENCLAYOUTUNIT_H

#include "d2string.h"
#include "d2vector.h"
#include "enclayout.h"
#include "midgardid.h"

namespace game {

struct IMidgardObjectMap;
struct IEncUnitDescriptor;
struct CEncParamBase;
struct IMqImage2;

struct CEncLayoutUnitData
{
    CMidgardID unitId;
    const IMidgardObjectMap* objectMap;
    int unknown;
    int unknown2;
    int unknown3;
    int unknown4;
    int unknown5;
    int unknown6;
    Vector<SmartPtr<IMqImage2>> leaderAbilityIcons;
    Vector<String> leaderAbilityTexts;
    IEncUnitDescriptor* unitDescriptor;
};

static_assert(sizeof(CEncLayoutUnitData) == 68,
              "Size of CEncLayoutUnitData structure must be exactly 68 bytes");

struct CEncLayoutUnit : public IEncLayout
{
    CEncLayoutUnitData* data;
};

static_assert(sizeof(CEncLayoutUnit) == 16,
              "Size of CEncLayoutUnit structure must be exactly 16 bytes");

struct CEncLayoutUnitVftable : CInterfaceVftable
{
    void* method35;
};

static_assert(sizeof(CEncLayoutUnitVftable) == 35 * sizeof(void*),
              "CEncLayoutUnit vftable must have exactly 35 methods");

namespace CEncLayoutUnitApi {

struct Api
{
    using Constructor = CEncLayoutUnit*(__thiscall*)(CEncLayoutUnit* thisptr,
                                                     const IMidgardObjectMap* objectMap,
                                                     CInterface* parent,
                                                     const CMqRect* area,
                                                     const CMidgardID* unitId,
                                                     const CEncParamBase* encParam,
                                                     const CMidgardID* playerId);
    Constructor constructor;

    using Constructor2 = CEncLayoutUnit*(__thiscall*)(CEncLayoutUnit* thisptr,
                                                      IEncUnitDescriptor* descriptor,
                                                      CInterface* parent,
                                                      const CMqRect* area,
                                                      const CEncParamBase* encParam);
    Constructor2 constructor2;

    using DataConstructor = CEncLayoutUnitData*(__thiscall*)(CEncLayoutUnitData* thisptr);
    DataConstructor dataConstructor;

    using DataDestructor = void(__thiscall*)(CEncLayoutUnitData* thisptr);
    DataDestructor dataDestructor;

    using Initialize = void(__thiscall*)(CEncLayoutUnit* thisptr, const CEncParamBase* encParam);
    Initialize initialize;

    using Update = void(__thiscall*)(CEncLayoutUnit* thisptr);
    Update update;
};

Api& get();

CEncLayoutUnitVftable* vftable();

} // namespace CEncLayoutUnitApi

} // namespace game

#endif // ENCLAYOUTUNIT_H
