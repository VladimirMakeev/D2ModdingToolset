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

#ifndef UMMODIFIER_H
#define UMMODIFIER_H

#include "midgardid.h"

namespace game {

struct CUmModifierVftable;
struct CUmModifierData;
struct ModifierValue;
struct GlobalData;
struct IUsUnit;
struct TUsUnitImpl;

struct CUmModifier
{
    const CUmModifierVftable* vftable;
    CUmModifierData* data;
};

static_assert(sizeof(CUmModifier) == 8, "Size of CUmModifier structure must be exactly 8 bytes");

struct ModifierValue
{
    bool initialized;
    char padding[3];
    int value;
};

static_assert(sizeof(ModifierValue) == 8,
              "Size of ModifierValue structure must be exactly 8 bytes");

/** Modifier element types (bitflags), corresponds to ModifierElementType enum (values are not
 * identical). */
// clang-format off
enum class ModifierElementTypeFlag : int
{
    // Used by CUmStack
    MoveAllowance       = 0b0000000000000001,
    ScoutingRange       = 0b0000000000000010,
    Leadership          = 0b0000000000000100,
    MoveAbility         = 0b0000000000001000,
    LeaderAbility       = 0b0000000000010000,
    // Used by CUmAttack
    QtyDamage           = 0b0000000000100000,
    Power               = 0b0000000001000000,
    Initiative          = 0b0000000010000000,
    // Used by CUmUnit
    Hp                  = 0b0000000100000000,
    Regeneration        = 0b0000001000000000,
    Armor               = 0b0000010000000000,
    ImmunityAlways      = 0b0000100000000000,
    ImmunityOnce        = 0b0001000000000000,
    ImmunityclassAlways = 0b0010000000000000,
    ImmunityclassOnce   = 0b0100000000000000,
    // Used by CUmAttack
    AttackDrain         = 0b1000000000000000,
};
// clang-format on

struct CUmModifierVftable
{
    using Destructor = void(__thiscall*)(CUmModifier* thisptr, bool freeMemory);
    Destructor destructor;

    using Method1 = int(__thiscall*)(CUmModifier* thisptr);
    Method1 method1;

    using Method2 = bool(__thiscall*)(CUmModifier* thisptr, int* a2);
    Method2 method2;

    using Method3 = bool(__thiscall*)(CUmModifier* thisptr, TUsUnitImpl* unitImpl);
    Method3 method3;

    using Method4 = bool(__thiscall*)(CUmModifier* thisptr, int a2);
    Method4 method4;

    using Method5 = bool(__thiscall*)(CUmModifier* thisptr);
    Method5 method5;

    using Method6 = bool(__thiscall*)(CUmModifier* thisptr);
    Method6 method6;

    using HasElement = bool(__thiscall*)(CUmModifier* thisptr, ModifierElementTypeFlag type);
    HasElement hasElement;

    using GetFirstElementValue = int(__thiscall*)(CUmModifier* thisptr);
    GetFirstElementValue getFirstElementValue;

    using GetDesc = const char*(__thiscall*)(CUmModifier* thisptr);
    GetDesc getDesc;

    using Method10 = int(__thiscall*)(CUmModifier* thisptr);
    Method10 method10;
};

struct CUmModifierData
{
    CUmModifier* next;
    IUsUnit* prev;
    CMidgardID modifierId;
};

static_assert(sizeof(CUmModifierData) == 12,
              "Size of CUmModifierData structure must be exactly 12 bytes");

namespace CUmModifierApi {

struct Api
{
    using Constructor = CUmModifier*(__thiscall*)(CUmModifier* thisptr,
                                                  const CMidgardID* modifierId,
                                                  const GlobalData** globalData);
    Constructor constructor;

    using CopyConstructor = CUmModifier*(__thiscall*)(CUmModifier* thisptr, const CUmModifier* src);
    CopyConstructor copyConstructor;
};

Api& get();

} // namespace CUmModifierApi

} // namespace game

#endif // UMMODIFIER_H
