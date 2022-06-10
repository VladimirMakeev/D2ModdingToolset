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
#include "d2map.h"
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

assert_size(CBatAttackDrainOverflow, 20);

/** Maps heal ammount to unit id. */
using DrainOverflowHealData = Map<CMidgardID, int>;
using DrainOverflowHealIterator = MapIterator<CMidgardID, int>;

namespace CBatAttackDrainOverflowApi {

struct Api
{
    /** Initializes heal data structure. */
    using HealDataConstructor = DrainOverflowHealData*(__thiscall*)(DrainOverflowHealData* thisptr);
    HealDataConstructor healDataCtor;

    /** Cleans up heal data structure. */
    using HealDataDestructor = void(__thiscall*)(DrainOverflowHealData* thisptr);
    HealDataDestructor healDataDtor;

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

IBatAttackVftable* vftable();

} // namespace CBatAttackDrainOverflowApi

} // namespace game

#endif // BATATTACKDRAINOVERFLOW_H
