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

#include "midgardid.h"
#include "midobject.h"

namespace game {

struct TUnitModifierVftable;
struct GlobalData;
struct CDBTable;
struct IUsUnit;
struct LModifGroup;
struct LUnitCategory;
struct CUmModifier;

struct TUnitModifier : IMidObjectT<TUnitModifierVftable>
{
    LModifGroup* group;
};

static_assert(sizeof(TUnitModifier) == 12,
              "Size of TUnitModifier structure must be exactly 12 bytes");

struct TUnitModifierVftable : IMidObjectVftable
{
    using CanApplyToStackWithLeadership = bool(__thiscall*)(TUnitModifier* thisptr,
                                                            const int* leadership);
    CanApplyToStackWithLeadership canApplyToStackWithLeadership;

    using CanApplyToUnit = bool(__thiscall*)(TUnitModifier* thisptr, const IUsUnit* unit);
    CanApplyToUnit canApplyToUnit;

    using CanApplyToUnitCategory = bool(__thiscall*)(TUnitModifier* thisptr,
                                                     const LUnitCategory* unitCategory);
    CanApplyToUnitCategory canApplyToUnitCategory;

    using Copy = CUmModifier*(__thiscall*)(TUnitModifier* thisptr);
    Copy copy;
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
