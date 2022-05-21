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

#ifndef UMATTACK_H
#define UMATTACK_H

#include "attackmodified.h"
#include "textandid.h"
#include "ummodifier.h"
#include "ussoldier.h"
#include "usunit.h"
#include <cstddef>

namespace game {

struct CUmAttackData;
struct CUmAttackDataPatched;
struct GlobalData;
struct CDBTable;

struct CUmAttack
{
    IUsUnit usUnit;
    IUsSoldier usSoldier;
    CUmModifier umModifier;
    CUmAttackDataPatched* data;
};

static_assert(sizeof(CUmAttack) == 24, "Size of CUmAttack structure must be exactly 24 bytes");

struct CUmAttackData
{
    ModifierValue qtyDamage;
    ModifierValue power;
    ModifierValue initiative;
    ModifierValue attackDrain;
    TextAndId desc;
    CAttackModified attackModified;
};

static_assert(sizeof(CUmAttackData) == 52,
              "Size of CUmAttackData structure must be exactly 52 bytes");

struct CUmAttackDataPatched : CUmAttackData
{
    CAttackModified altAttackModified;
};

static inline CUmAttack* castSoldierToUmAttack(const IUsSoldier* soldier)
{
    return reinterpret_cast<CUmAttack*>((std::uintptr_t)soldier - offsetof(CUmAttack, usSoldier));
}

namespace CUmAttackApi {

struct Api
{
    using Constructor = CUmAttack*(__thiscall*)(CUmAttack* thisptr,
                                                const CMidgardID* modifierId,
                                                CDBTable* dbTable,
                                                const GlobalData** globalData);
    Constructor constructor;

    using CopyConstructor = CUmAttack*(__thiscall*)(CUmAttack* thisptr, const CUmAttack* src);
    CopyConstructor copyConstructor;

    using DataConstructor = CUmAttackData*(__thiscall*)(CUmAttackData* thisptr);
    DataConstructor dataConstructor;

    using DataCopyConstructor = CUmAttackData*(__thiscall*)(CUmAttackData* thisptr,
                                                            const CUmAttackData* src);
    DataCopyConstructor dataCopyConstructor;

    using ReadData = void(__stdcall*)(CDBTable* dbTable,
                                      CUmAttackData* value,
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

} // namespace CUmAttackApi

} // namespace game

#endif // UMATTACK_H
