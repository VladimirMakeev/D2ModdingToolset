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

#ifndef DYNAMICCAST_H
#define DYNAMICCAST_H

#include <cstdint>

namespace game {

/**
 * RTTI type descriptor structure used in game.
 * Must be aligned on 8 byte boundary.
 * @see http://www.openrce.org/articles/full_view/21 for additional info.
 */
struct alignas(8) TypeDescriptor
{
    const void* vftable; /**< Vtable of type_info class. */
    void* spare;         /**< Used to keep the demangled name returned by type_info::name(). */

// Nonstandard extension: zero-sized array in struct/union
#pragma warning(suppress : 4200)
    char name[0]; /**< Mangled type name. */
};

struct BaseClassDescriptor;

struct BaseClassArray
{
// Nonstandard extension: zero-sized array in struct/union
#pragma warning(suppress : 4200)
    BaseClassDescriptor* baseClasses[];
};

/** Describes inheritance hierarchy of a class. */
struct ClassHierarchyDescriptor
{
    std::uint32_t signature;
    std::uint32_t attributes;
    std::uint32_t numBaseClasses; /**< Number of elements in baseClassArray. */
    BaseClassArray* baseClassArray;
};

/**
 * RTTI complete object locator structure used in game.
 * Allows compiler to find the location of the complete object from a specific vftable pointer.
 * Pointer to this structure can be found in memory just before class vftable.
 * @see http://www.openrce.org/articles/full_view/23 for additional info.
 */
struct CompleteObjectLocator
{
    std::uint32_t signature;
    std::uint32_t offset;                      /**< Offset of this vftable in complete class. */
    std::uint32_t cdOffset;                    /**< Constructor displacement offset. */
    TypeDescriptor* typeDescriptor;            /**< type_info* of the complete class. */
    ClassHierarchyDescriptor* classDescriptor; /**< Describes inheritance hierarchy. */
};

template <typename T>
struct RttiInfo
{
    const CompleteObjectLocator* locator;
    T vftable;
};

namespace RttiApi {

struct Api
{
    /**
     * Runtime implementation of dynamic_cast operator used in game.
     * @param[in] ptr pointer to a polymorphic object.
     * @param vfDelta offset of virtual function pointer in object.
     * @param[in] srcType static type of object pointed to by the ptr.
     * @dstType[in] intended result of a cast.
     * @isReference true if input is a reference.
     * @returns pointer to object of type dstType or NULL.
     */
    using DynamicCast = void*(__cdecl*)(const void* ptr,
                                        int vfDelta,
                                        const TypeDescriptor* srcType,
                                        const TypeDescriptor* dstType,
                                        int isReference);
    DynamicCast dynamicCast;

    using TypeInfoRawName = const char*(__thiscall*)(const TypeDescriptor* thisptr);
    TypeInfoRawName* typeInfoRawName;

    using TypeInfoInequalityOperator = int(__thiscall*)(const TypeDescriptor* thisptr,
                                                        const TypeDescriptor* other);
    TypeInfoInequalityOperator* typeInfoInequalityOperator;

    using TypeIdOperator = const TypeDescriptor*(__cdecl*)(const void* ptr);
    TypeIdOperator* typeIdOperator;
};

Api& get();

struct Rtti
{
    TypeDescriptor* IMidScenarioObjectType;
    TypeDescriptor* CMidPlayerType;
    TypeDescriptor* CPlayerBuildingsType;
    TypeDescriptor* TBuildingTypeType;
    TypeDescriptor* TBuildingUnitUpgTypeType;
    TypeDescriptor* CMidStackType;
    TypeDescriptor* CUmModifierType;
    TypeDescriptor* CUmUnitType;
    TypeDescriptor* CNetMsgType;
    TypeDescriptor* CCommandMsgType;
    TypeDescriptor* CStackBattleActionMsgType;
    TypeDescriptor* CCmdBattleStartMsgType;
    TypeDescriptor* CCmdBattleChooseActionMsgType;
    TypeDescriptor* CCmdBattleResultMsgType;
    TypeDescriptor* CCmdBattleEndMsgType;
    TypeDescriptor* CCommandMsgTempl_8_1_Type;  // CCmdBattleStartMsg
    TypeDescriptor* CCommandMsgTempl_9_0_Type;  // CCmdBattleChooseActionMsg
    TypeDescriptor* CCommandMsgTempl_10_0_Type; // CCmdBattleResultMsg
    TypeDescriptor* CCommandMsgTempl_11_2_Type; // CCmdBattleEndMsg
    TypeDescriptor* CMidgardMapType;
    TypeDescriptor* IAttackType;
    TypeDescriptor* CAttackImplType;
    TypeDescriptor* CAttackModifiedType;
    TypeDescriptor* IItemType;
    TypeDescriptor* CItemBattleType;
    TypeDescriptor* IUsSoldierType;
    TypeDescriptor* TUsSoldierImplType;
    TypeDescriptor* IBatAttackType;
    TypeDescriptor* CBatAttackTransformSelfType;
    TypeDescriptor* CMidLocationType;
    TypeDescriptor* TUsRacialSoldierType;
    TypeDescriptor* IUsStackLeaderType;
    TypeDescriptor* IUsUnitType;
    TypeDescriptor* IEncUnitDescriptorType;
    TypeDescriptor* CMidUnitDescriptorType;
    TypeDescriptor* CUmAttackType;
    TypeDescriptor* CUnitTypeDescriptorType;
    TypeDescriptor* CLeaderUnitDescriptorType;
    TypeDescriptor* CBatAttackUseOrbType;
    TypeDescriptor* CBatAttackUseTalismanType;
    TypeDescriptor* CItemEquipmentType;
    TypeDescriptor* CUmStackType;
};

const Rtti& rtti();

} // namespace RttiApi

} // namespace game

#endif // DYNAMICCAST_H
