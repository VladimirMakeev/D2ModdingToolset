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

#ifndef BATTLEATTACKINFO_H
#define BATTLEATTACKINFO_H

#include "linkedlist.h"
#include "midgardid.h"
#include <cstddef>

namespace game {

/** Information about attack results for a specific unit in battle. */
struct BattleAttackUnitInfo
{
    CMidgardID unitId;
    CMidgardID unitImplId;
    bool attackMissed;
    bool defend;
    bool targetImmune;
    char padding;
    int damage;
    int criticalDamage;
};

static_assert(sizeof(BattleAttackUnitInfo) == 20,
              "Size of BattleAttackUnitInfo structure must be exactly 20 bytes");

using AttackUnitInfoList = LinkedList<BattleAttackUnitInfo>;

/**
 * Holds information about performed attack.
 * Used to update battle ui.
 */
struct BattleAttackInfo
{
    CMidgardID unitId;
    bool attacker;
    char padding[3];
    int unknown;
    int attackNumber;
    CMidgardID id;
    AttackUnitInfoList unitsInfo;
};

static_assert(sizeof(BattleAttackInfo) == 36,
              "Size of BattleAttackInfo structure must be exactly 36 bytes");

static_assert(offsetof(BattleAttackInfo, unitsInfo) == 20,
              "BattleAttackInfo::unitsInfo offset must be 20 bytes");

namespace BattleAttackInfoApi {

struct Api
{
    /** Adds unit specific attack information to the list. */
    using AddUnitInfo = void(__thiscall*)(AttackUnitInfoList* thisptr,
                                          const BattleAttackUnitInfo* info);
    AddUnitInfo addUnitInfo;
};

Api& get();

} // namespace BattleAttackInfoApi

} // namespace game

#endif // BATTLEATTACKINFO_H
