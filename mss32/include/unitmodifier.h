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

#ifndef UNITMODIFIER_H
#define UNITMODIFIER_H

#include "midobject.h"
#include "modifgroup.h"

namespace game {

struct TUnitModifierData;
struct TUnitModifierVftable;
struct GlobalData;
struct CDBTable;
struct IUsUnit;
struct LUnitCategory;
struct CUmModifier;

/**
 * Serves as single modifier factory. Id holds an id of modifier that this factory produces.
 * Reads modifier from DBF, then holds it as global instance and redirects all the calls to it.
 * Provides access to the global instance in cases where modifier is used globally (city armor
 * bonus, some spell effects, etc.) or to check if it can be applied.
 * Uses CUmModifier::Copy to create new instances to be applied to individual units.
 */
struct TUnitModifier : IMidObjectT<TUnitModifierVftable>
{
    TUnitModifierData* data;
};

static_assert(sizeof(TUnitModifier) == 12,
              "Size of TUnitModifier structure must be exactly 12 bytes");

struct TUnitModifierData
{
    LModifGroup group;
    CUmModifier* modifier;
};

static_assert(sizeof(TUnitModifierData) == 16,
              "Size of TUnitModifierData structure must be exactly 16 bytes");

struct TUnitModifierVftable : IMidObjectVftable
{
    using CanApplyWithLeadership = bool(__thiscall*)(const TUnitModifier* thisptr,
                                                     const int* leadership);
    CanApplyWithLeadership canApplyWithLeadership;

    using CanApplyToUnit = bool(__thiscall*)(const TUnitModifier* thisptr, const IUsUnit* unit);
    CanApplyToUnit canApplyToUnit;

    using CanApplyToUnitCategory = bool(__thiscall*)(const TUnitModifier* thisptr,
                                                     const LUnitCategory* unitCategory);
    CanApplyToUnitCategory canApplyToUnitCategory;

    using CreateModifier = CUmModifier*(__thiscall*)(const TUnitModifier* thisptr);
    CreateModifier createModifier;
};

static_assert(sizeof(TUnitModifierVftable) == 5 * sizeof(void*),
              "TUnitModifier vftable must have exactly 5 methods");

namespace TUnitModifierApi {

struct Api
{
    using Constructor = TUnitModifier*(__thiscall*)(TUnitModifier* thisptr,
                                                    const CDBTable* dbTable,
                                                    const char* globalsFolderPath,
                                                    void* codeBaseEnvProxy,
                                                    const GlobalData** globalData);
    Constructor constructor;
};

Api& get();

const TUnitModifierVftable* vftable();

} // namespace TUnitModifierApi

} // namespace game

#endif // UNITMODIFIER_H
