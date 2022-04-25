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

#include "modifgroup.h"
#include "usunitimpl.h"

namespace game {

struct TUnitModifierVftable;
struct GlobalData;
struct CDBTable;

struct TUnitModifier
{
    const TUnitModifierVftable* vftable;
    CMidgardID id;
    LModifGroup* group;
};

struct TUnitModifierVftable
{
    using Destructor = void(__thiscall*)(TUnitModifier* thisptr, bool freeMemory);
    Destructor destructor;

    using Method1 = int(__thiscall*)(TUnitModifier* thisptr, int a2);
    Method1 method1;

    using IsApplicable = bool(__thiscall*)(TUnitModifier* thisptr, TUsUnitImpl* unitImpl);
    IsApplicable isApplicable;

    using Method3 = int(__thiscall*)(TUnitModifier* thisptr, int a2);
    Method3 method3;

    using Method4 = int(__thiscall*)(TUnitModifier* thisptr);
    Method4 method4;
};

static_assert(sizeof(TUnitModifier) == 12,
              "Size of TUnitModifier structure must be exactly 12 bytes");

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
