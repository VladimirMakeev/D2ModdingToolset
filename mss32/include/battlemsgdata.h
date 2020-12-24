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

#ifndef BATTLEMSGDATA_H
#define BATTLEMSGDATA_H

#include "midgardid.h"
#include <cstddef>
#include <cstdint>

namespace game {

struct IMidgardObjectMap;

/** Unit statuses in battle. */
enum class BattleStatus : int
{
    Status0 = 0,
    Dead,
    SkipTurn,            /**< Unit skips his turn (paralyzed or petrified). */
    Petrify,             /**< Unit petrified. */
    SkipLong,            /**< Both paralyze and petrify sets this in case of 'infinite' attack. */
    BoostDamageLvl1,     /**< 25% boost */
    BoostDamageLvl2,     /**< 50% boost */
    BoostDamageLvl3,     /**< 75% boost */
    BoostDamageLvl4,     /**< 100% boost */
    BoostDamageLong,     /**< Long damage boost (until battle is over or lower damage applied). */
    LowerDamageLvl1,     /**< 50% lower damage */
    LowerDamageLvl2,     /**< 33% lower damage */
    LowerDamageLong,     /**< Long lower damage (until battle is over or removed). */
    LowerInitiative,     /**< 50% lower initiative */
    LowerInitiativeLong, /**< Long lower initiative. */
    Poison,              /**< Poison dot. */
    PoisonLong,          /**< Long poison applied. */
    Frostbite,           /**< Frostbite dot. */
    FrostbiteLong,       /**< Long frostbite applied. */
    Blister,             /**< Blister dot. */
    BlisterLong,         /**< Long blister applied. */
    Status21,            /**< cure applied ? */
    Transform,           /**< Unit transformed by another unit. */
    TransformLong,       /**< Long transformation applied by another unit. */
    Status24,            /**< Unit transfomed himself ? */
    Status25,            /**< Doppelganger transformation ? */
    Status26,            /**< Drain level applied ? */
    Status27,            /**< Unit was summoned during battle ? */
    Status28,            /**< Fast retreat ? */
    Retreat,             /**< Unit retreating. */
    Status30,
    Defend,  /**< Defend was used in this round. */
    Status32 /**< unsummon effect applied ? */
};

struct UnknownUnitInfo
{
    CMidgardID id1;
    CMidgardID id2;
};

static_assert(sizeof(UnknownUnitInfo) == 8,
              "Size of UnknownUnitInfo structure must be exactly 8 bytes");

/** Battle turn info. */
struct BattleTurn
{
    /** Id of unit performing the turn. */
    CMidgardID unitId;
    /** Number of attacks unit can make in a single turn. 2 for units with double attack. */
    char attacksCount;
    char padding[3];
};

static_assert(sizeof(BattleTurn) == 8, "Size of BattleTurn structure must be exactly 8 bytes");

/** Additional unit statuses and information packed in single byte. */
union UnitFlags
{
    struct
    {
        std::uint8_t indexInGroup : 3;
        bool attacker : 1;
        std::uint8_t flag4 : 1;
        std::uint8_t unknown : 1;
        bool revived : 1;
        std::uint8_t flag7 : 1;
    } parts;

    std::uint8_t value;
};

static_assert(sizeof(UnitFlags) == 1, "Size of UnitFlags union must be exactly 1 byte");

/** Holds unit information used in battle. */
struct UnitInfo
{
    CMidgardID unitId1;
    CMidgardID unitId2;
    /** Bitmask made of BattleStatus values used as shifts. */
    std::uint64_t unitStatuses;
    char unknown[20];
    /** Bitmask with values for each of LAttackSource. */
    std::uint8_t attackSourceImmunityStatuses;
    char padding[3];
    /** Bitmask with values for each of LAttackClass. */
    std::uint32_t attackClassImmunityStatuses;
    std::int16_t unitHp;
    std::int16_t unitXp;
    UnitFlags unitFlags;
    char unknown2;
    char unknown5;
    /** Round when long poison was applied. 0 means poison was not applied. */
    std::int8_t poisonAppliedRound;
    /** Round when long frostbite was applied. */
    std::int8_t frostbiteAppliedRound;
    /** Round when long blister was applied. */
    std::int8_t blisterAppliedRound;
    /** Round when long transform was applied. */
    std::int8_t transformAppliedRound;
    char unknown6[5];
    UnknownUnitInfo elements[8];
    CMidgardID unknownIds[8];
    int shatteredArmor;
    int fortificationArmor;
    int unknown7;
};

static_assert(sizeof(UnitInfo) == 168, "Size of UnitInfo structure must be exactly 168 bytes");

static_assert(offsetof(UnitInfo, unitStatuses) == 8,
              "UnitInfo::unitStatuses offset must be 8 bytes");

static_assert(offsetof(UnitInfo, unitHp) == 44, "UnitInfo::unitHp offset must be 44 bytes");

static_assert(offsetof(UnitInfo, unitXp) == 46, "UnitInfo::unitXp offset must be 46 bytes");

static_assert(offsetof(UnitInfo, unitFlags) == 48, "UnitInfo::unitFlags offset must be 48 bytes");

static_assert(offsetof(UnitInfo, poisonAppliedRound) == 51,
              "UnitInfo::poisonAppliedRound offset must be 51 bytes");

static_assert(offsetof(UnitInfo, elements) == 60, "UnitInfo::elements offset must be 60 bytes");

static_assert(offsetof(UnitInfo, shatteredArmor) == 156,
              "UnitInfo::shatteredArmor offset must be 156 bytes");

/**
 * Common part of the network messages that is being sent during battle.
 */
struct BattleMsgData
{
    /**
     * 12 for alive units, another 12 is for units that can be possibly summoned over dead bodies
     * minus 2 because each group can only have 5 dead units, the 6-th one must be alive or the
     * battle is over.
     */
    UnitInfo unitsInfo[22];
    /** Order of turns in a single battle round. 12 for units and 1 reserved? */
    BattleTurn turnsOrder[13];
    CMidgardID attackerGroupId;
    /** Can be id of a stack, fortification or ruin. */
    CMidgardID defenderGroupId;
    CMidgardID attackerPlayerId;
    CMidgardID defenderPlayerId;
    CMidgardID attackerStackUnitIds[6];
    CMidgardID defenderStackUnitIds[6];
    CMidgardID ids3[2];
    CMidgardID ids4[4];
    std::int8_t currentRound; /**< Round counting starts from 1. */
    char padding[3];
    int unknown3;
    int unknown4;
    int unknown5;
    int unknown6;
    int unknown7;
    char unknown8;
    char unknown9;
    char unknown10;
    char padding2;
    char unknown11[4];
};

static_assert(sizeof(BattleMsgData) == 3920,
              "Size of BattleMsgData structure must be exactly 3920 bytes");

static_assert(offsetof(BattleMsgData, turnsOrder) == 3696,
              "BattleMsgData::turnsOrder offset must be 3696 bytes");

static_assert(offsetof(BattleMsgData, attackerStackUnitIds) == 3816,
              "BattleMsgData::attackerStackUnitIds offset must be 3816 bytes");

namespace BattleMsgDataApi {

struct Api
{
    using GetUnitStatus = bool(__thiscall*)(const BattleMsgData* thisptr,
                                            const CMidgardID* unitId,
                                            BattleStatus status);
    GetUnitStatus getUnitStatus;

    using GetUnitIntValue = int(__thiscall*)(const BattleMsgData* thisptr,
                                             const CMidgardID* unitId);
    GetUnitIntValue getUnitShatteredArmor;
    GetUnitIntValue getUnitFortificationArmor;

    using SetUnitIntValue = void(__thiscall*)(BattleMsgData* thisptr,
                                              const CMidgardID* unitId,
                                              int value);
    SetUnitIntValue setUnitShatteredArmor;

    /** Sets UnitInfo::unitHp for unit with unitId to specified value. */
    using SetUnitInt16Value = void(__thiscall*)(BattleMsgData* thisptr,
                                                const CMidgardID* unitId,
                                                std::int16_t value);
    SetUnitInt16Value setUnitHp;

    /** Checks if unit is dead, updates UnitInfo::unitStatuses accordingly. */
    using CheckUnitDeath = void(__stdcall*)(const IMidgardObjectMap* objectMap,
                                            BattleMsgData* battleMsgData,
                                            const CMidgardID* unitId);
    CheckUnitDeath checkUnitDeath;
};

Api& get();

} // namespace BattleMsgDataApi

} // namespace game

#endif // BATTLEMSGDATA_H
