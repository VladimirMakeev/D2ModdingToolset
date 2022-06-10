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

#ifndef UMUNIT_H
#define UMUNIT_H

#include "smartptr.h"
#include "textandid.h"
#include "ummodifier.h"
#include "ussoldier.h"
#include "usunit.h"
#include <cstddef>

namespace game {

struct CUmUnitData;
struct GlobalData;
struct CDBTable;

struct CUmUnit
{
    IUsUnit usUnit;
    IUsSoldier usSoldier;
    CUmModifier umModifier;
    CUmUnitData* data;
};

assert_size(CUmUnit, 24);

struct CUmUnitData
{
    ModifierValue hp;
    bool isPercentHp;
    char padding[3];
    ModifierValue regeneration;
    ModifierValue armor;
    SmartPtr<LAttackSource> immunity;
    SmartPtr<LImmuneCat> immuneCat;
    SmartPtr<LAttackClass> immunityC;
    SmartPtr<LImmuneCat> immuneCatC;
    TextAndId desc;
};

assert_size(CUmUnitData, 68);

static inline CUmUnit* castSoldierToUmUnit(const IUsSoldier* soldier)
{
    return reinterpret_cast<CUmUnit*>((uintptr_t)soldier - offsetof(CUmUnit, usSoldier));
}

namespace CUmUnitApi {

struct Api
{
    using Constructor = CUmUnit*(__thiscall*)(CUmUnit* thisptr,
                                              const CMidgardID* modifierId,
                                              CDBTable* dbTable,
                                              const GlobalData** globalData);
    Constructor constructor;

    using CopyConstructor = CUmUnit*(__thiscall*)(CUmUnit* thisptr, const CUmUnit* src);
    CopyConstructor copyConstructor;

    using DataConstructor = CUmUnitData*(__thiscall*)(CUmUnitData* thisptr);
    DataConstructor dataConstructor;

    using DataCopyConstructor = CUmUnitData*(__thiscall*)(CUmUnitData* thisptr,
                                                          const CUmUnitData* src);
    DataCopyConstructor dataCopyConstructor;

    using ReadData = void(__stdcall*)(CDBTable* dbTable,
                                      CUmUnitData* value,
                                      const GlobalData** globalData);
    ReadData readData;
};

Api& get();

struct Vftable
{
    const IUsUnitVftable* usUnit;
    const IUsSoldierVftable* usSoldier;
    const CUmModifierVftable* umModifier;
};

Vftable& vftable();

} // namespace CUmUnitApi

} // namespace game

#endif // UMUNIT_H
