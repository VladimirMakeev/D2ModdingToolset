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

#ifndef UNITGENERATOR_H
#define UNITGENERATOR_H

namespace game {

struct CMidgardID;
struct CUnitGeneratorVftable;

/**
 * Used to create unit implementations in game.
 * By default, unit implementation objects are created from records of GUnits.dbf.
 * For example, objects of types TUsUnitImpl or TUsSoldierImpl is a unit implementations.
 * Creation, promotion or modification of units in groups (stacks, fortifications and ruins)
 * is based on implementation data they refer to.
 * Unit promotions and modifications create new implementations.
 */
struct CUnitGenerator
{
    CUnitGeneratorVftable* vftable;
};

struct CUnitGeneratorVftable
{
    void* destructor;

    /**
     * Generates new unit implementation id from specified unitImplId and level.
     * @param[in] thisptr pointer to unit generator instance.
     * @param[inout] resultUnitImplId pointer where new implementation id will be stored.
     * @param[in] unitImplId unit implementation id.
     */
    using GenerateUnitImplId = CMidgardID*(__thiscall*)(CUnitGenerator* thisptr,
                                                        CMidgardID* resultUnitImplId,
                                                        const CMidgardID* unitImplId,
                                                        int unitLevel);
    GenerateUnitImplId generateUnitImplId;

    /**
     * Checks if unit implementation with specified id exists, if not creates a new implementation.
     * @returns true if unit implementation exists or was successfully created.
     */
    using GenerateUnitImpl = bool(__thiscall*)(CUnitGenerator* thisptr,
                                               const CMidgardID* unitImplId);
    GenerateUnitImpl generateUnitImpl;

    /** Returns true if unit implementation id has IdType::UnitGenerated. */
    using IsUnitGenerated = bool(__thiscall*)(CUnitGenerator* thisptr, CMidgardID* unitImplId);
    IsUnitGenerated isUnitGenerated;

    /**
     * Used in doppelganger attack logic.
     * Checks if a unit implementation with the specified id is global, if so - returns the id as
     * is. Otherwise, if the id belongs to a generated unit, returns an id of the corresponding
     * global unit. If the id is nor a global unit nor a generated unit, returns the empty id.
     */
    using GetGlobalUnitImplId = CMidgardID*(__thiscall*)(CUnitGenerator* thisptr,
                                                         CMidgardID* resultUnitImplId,
                                                         const CMidgardID* unitImplId);
    GetGlobalUnitImplId getGlobalUnitImplId;

    /** Assumption: returns number of existing unit implementations. */
    using GetUnitImplsTotal = int(__thiscall*)(CUnitGenerator* thisptr);
    GetUnitImplsTotal getUnitImplsTotal;
};

static_assert(sizeof(CUnitGeneratorVftable) == 6 * sizeof(void*),
              "CUnitGenerator vftable must have exactly 6 methods");

} // namespace game

#endif // UNITGENERATOR_H
