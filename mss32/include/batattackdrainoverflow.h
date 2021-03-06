/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#ifndef BATATTACKDRAINOVERFLOW_H
#define BATATTACKDRAINOVERFLOW_H

#include "batattack.h"
#include "midgardid.h"

namespace game {

struct IAttack;
struct IMidgardObjectMap;

/** Represents vampiric attacks that can heal allies. */
struct CBatAttackDrainOverflow : public CBatAttackBase
{
    CMidgardID unitId;
    CMidgardID id2;
    int attackNumber;
    IAttack* attack;
};

static_assert(sizeof(CBatAttackDrainOverflow) == 20,
              "Size of CBatAttackDrainOverflow structure must be exactly 20 bytes");

/** Used to compute heal for each unit in allied stack. */
struct DrainOverflowHealData
{
    char unknown[28];
};

static_assert(sizeof(DrainOverflowHealData) == 28,
              "Size of DrainOverflowHealData structure must be exactly 28 bytes");

/** Used to traverse heal data. */
struct DrainOverflowHealIterator
{
    int unknown[3];
};

static_assert(sizeof(DrainOverflowHealIterator) == 12,
              "Size of DrainOverflowHealIterator structure must be exactly 12 bytes");

namespace CBatAttackDrainOverflowApi {

struct Api
{
    IBatAttackVftable::OnAttack onHit;

    /** Initializes heal data structure. */
    using HealDataConstructor = DrainOverflowHealData*(__thiscall*)(DrainOverflowHealData* thisptr);
    HealDataConstructor healDataCtor;

    /** Cleans up heal data structure. */
    using HealDataDestructor = void(__thiscall*)(DrainOverflowHealData* thisptr);
    HealDataDestructor healDataDtor;

    /** Creates iterator. */
    using HealDataIteratorConstructor = DrainOverflowHealIterator*(
        __thiscall*)(DrainOverflowHealData* thisptr, DrainOverflowHealIterator* iterator);
    HealDataIteratorConstructor healDataIteratorCtor;

    /** Creates end iterator. */
    using HealDataEndIteratorConstructor = DrainOverflowHealIterator*(
        __thiscall*)(DrainOverflowHealData* thisptr, DrainOverflowHealIterator* endIterator);
    HealDataEndIteratorConstructor healDataEndIteratorCtor;

    /** Creates iterator from 'other'. */
    using HealDataIteratorCopyConstructor = DrainOverflowHealIterator*(
        __thiscall*)(DrainOverflowHealIterator* thisptr, const DrainOverflowHealIterator* other);
    HealDataIteratorCopyConstructor healDataIteratorCopyCtor;

    /** Returns true if iterator reached end. */
    using IsHealDataIteratorAtEnd = bool(__thiscall*)(const DrainOverflowHealIterator* thisptr,
                                                      const DrainOverflowHealIterator* endIterator);
    IsHealDataIteratorAtEnd isHealDataIteratorAtEnd;

    /** Advances iterator towards end. */
    using HealDataIteratorAdvance = DrainOverflowHealIterator*(
        __thiscall*)(DrainOverflowHealIterator* thisptr, DrainOverflowHealIterator* tmp, int a3);
    HealDataIteratorAdvance healDataIteratorAdvance;

    /** Used to access data the iterator currently pointing at. */
    using HealDataIteratorGetData = void*(__thiscall*)(const DrainOverflowHealIterator* thisptr);
    HealDataIteratorGetData healDataIteratorGetData;

    /**
     * Computes heal for units in specified group from drain overflow.
     * @param[inout] healData pointer to structure to store heal info.
     * @param[in] objectMap map to search units.
     * @param[in] groupId id of group to heal.
     * @param[in] attackerUnitId id of unit performing drain overflow attack. It will be excluded
     * from computation.
     * @param drainOverflow drain overflow value that will be converted to heal.
     * @returns number of units that can be healed, or zero.
     */
    using ComputeDrainOverflowGroupHeal = int(__stdcall*)(DrainOverflowHealData* healData,
                                                          const IMidgardObjectMap* objectMap,
                                                          const CMidgardID* groupId,
                                                          const CMidgardID* attackerUnitId,
                                                          int drainOverflow);
    ComputeDrainOverflowGroupHeal computeDrainOverflowGroupHeal;
};

Api& get();

} // namespace CBatAttackDrainOverflowApi

} // namespace game

#endif // BATATTACKDRAINOVERFLOW_H
