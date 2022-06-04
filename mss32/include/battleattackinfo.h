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

#include "d2assert.h"
#include "d2list.h"
#include "midgardid.h"
#include <cstddef>

namespace game {

/** Used by CBattleViewerInterf to show effects and play sounds. */
enum class AttackEffect : int
{
    Damage,        /**< Assumption: unit was damaged by common attack. */
    Paralyze,      /**< Unit paralyzed and passes its turn. Shows message with id 'X008TA0010'. */
    Retreating,    /**< Unit retreating. Unit image in battlefield flips horizontally. */
    Retreated,     /**< Unit retreated from battle. Unit disappears from battlefield. */
    BreakParalyze, /**< Paralyze is over. Shows message with id 'X008TA0024'. */
    BreakPetrify,  /**< Petrify is over. Shows message with id 'X008TA0025'. */
    Poison,        /**< Unit takes damage from poison. Shows message with id 'X008TA0014'. */
    Frostbite,     /**< Unit takes damage from frostbite. Shows message with id 'X008TA0015'. */
    Blister,       /**< Unit takes damage from blister. Shows message with id 'X160TA0022'. */
    Wait,          /**< Unit is waiting. Shows message with id 'X008TA0020'. */
    Defend,        /**< Unit defending. Shows message with id 'X008TA0021'. */
    Effect11,
    Effect12,      /**< Assumption: unit hit by attack or item used. */
    Untransform,   /**< Unit restores original form, except drain level. Message id 'X008TA0023'. */
    Unsummon,      /**< Summoned unit unsummoned. */
    GroupUpgrade,  /**< Unit gets promotion when battle won or gains experience.
                    * Message id 'X008TA0005'.
                    */
    LevelRestored, /**< Unit restores original form after drain level. Message id 'X008TA0029'. */
    Effect17,
    Effect18 /**< Set as default in BattleAttackInfo constructor. */
};

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

assert_size(BattleAttackUnitInfo, 20);

using AttackUnitInfoList = List<BattleAttackUnitInfo>;

/**
 * Holds information about performed attack.
 * Used to update battle ui.
 */
struct BattleAttackInfo
{
    CMidgardID unitId;
    bool attacker;
    char padding[3];
    AttackEffect effect;
    int attackNumber;
    CMidgardID id;
    AttackUnitInfoList unitsInfo;
};

assert_size(BattleAttackInfo, 36);
assert_offset(BattleAttackInfo, unitsInfo, 20);

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
