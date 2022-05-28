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
struct LUnitCategory;

/**
 * Stored as global instance by TUnitModifier.
 * Most of the methods can be called on that instance (which is not applied to unit),
 * to check it before applying, or for specific needs like providing city armor bonus.
 * A copy is created every time it needs to be applied to unit.
 */
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

/**
 * Modifier element types (bitflags).
 * Correspond to ModifierElementType enum (values are not identical).
 */
// clang-format off
enum class ModifierElementTypeFlag : int
{
    None                = 0b0000000000000000,
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

    /** Called on global instance to create a copy to be applied to a unit. */
    using Copy = CUmModifier*(__thiscall*)(const CUmModifier* thisptr);
    Copy copy;

    /**
     * Called on global instance to check if can be applied if stack leader has specified
     * leadership.
     */
    using CanApplyWithLeadership = bool(__thiscall*)(const CUmModifier* thisptr,
                                                     const int* leadership);
    CanApplyWithLeadership canApplyWithLeadership;

    /** Called on global instance to check if it can be applied to unit. */
    using CanApplyToUnit = bool(__thiscall*)(const CUmModifier* thisptr, const IUsUnit* unit);
    CanApplyToUnit canApplyToUnit;

    /** Called on global instance (for example, to check if banner can have it). */
    using CanApplyToUnitCategory = bool(__thiscall*)(const CUmModifier* thisptr,
                                                     const LUnitCategory* unitCategory);
    CanApplyToUnitCategory canApplyToUnitCategory;

    /** Called on global instance (for example, to check if it matches spell category). */
    using GetBool = bool(__thiscall*)(const CUmModifier* thisptr);
    GetBool isLower;
    GetBool isBoost;

    /** Called on global instance (for example, to add city armor bonus). */
    using HasElement = bool(__thiscall*)(const CUmModifier* thisptr, ModifierElementTypeFlag type);
    HasElement hasElement;

    /** Called on global instance (for example, to add city armor bonus). */
    using GetFirstElementValue = int(__thiscall*)(const CUmModifier* thisptr);
    GetFirstElementValue getFirstElementValue;

    using GetDescription = const char*(__thiscall*)(const CUmModifier* thisptr);
    GetDescription getDescription;

    /** Takes unit impl id from previous unit and sets it for self. */
    using UpdateUnitImplId = void(__thiscall*)(CUmModifier* thisptr);
    UpdateUnitImplId updateUnitImplId;
};

static_assert(sizeof(CUmModifierVftable) == 11 * sizeof(void*),
              "CUmModifier vftable must have exactly 11 methods");

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

    using Destructor = void(__thiscall*)(CUmModifier* thisptr);
    Destructor destructor;

    using SetPrev = void(__thiscall*)(CUmModifier* thisptr, const IUsUnit* value);
    SetPrev setPrev;
};

Api& get();

} // namespace CUmModifierApi

} // namespace game

#endif // UMMODIFIER_H
