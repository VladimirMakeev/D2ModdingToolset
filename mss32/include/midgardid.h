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

#ifndef MIDGARDID_H
#define MIDGARDID_H

namespace game {

enum class IdCategory : int
{
    Global = 0, /**< Global entities, mainly from dbf files in globals game folder. */
    Campaign,   /**< Campaign related entities. */
    Scenario,   /**< Scenario entities and objects. */
    External,   /**< User interface messages, text, etc. */
    Invalid
};

enum class IdType : int
{
    Empty = 0,       /**< Type of empty id 'g000000000' when converted from string.
                      * Can not be used to construct id from parts.
                      */
    ApplicationText, /**< 'TA', entries of TApp.dbf and TAppEdit.dbf. */
    Building,        /**< 'BB', entries of Gbuild.dbf. */
    Race,            /**< 'RR' */
    Lord,            /**< 'LR' */
    Spell,           /**< 'SS' */
    UnitGlobal,      /**< 'UU', units implementations, entries of Gunits.dbf. */
    UnitGenerated,   /**< 'UG', runtime-generated units implementations. */
    UnitModifier,    /**< 'UM' */
    Attack,          /**< 'AA' */
    TextGlobal,      /**< 'TG', entries of Tglobal.dbf. */
    LandmarkGlobal,  /**< 'MG', entries of GLmark.dbf. */
    ItemGlobal,      /**< 'IG', entries of GItem.dbf. */
    NobleAction,     /**< 'NA', entries of Gaction.dbf. */
    DynamicUpgrade,  /**< 'DU' */
    // 15 for DA
    // 16 for AL
    // 17 for DC
    // 18 for AC
    CampaignFile = 19, /**< 'CC' */
    // 20 for CW
    // 21 for CO
    Plan = 22,         /**< 'PN' */
    ObjectCount,       /**< 'OB', number of objects in scenario file. */
    ScenarioFile,      /**< 'SC' */
    Map,               /**< 'MP' */
    MapBlock,          /**< 'MB' */
    ScenarioInfo,      /**< 'IF' */
    SpellEffects,      /**< 'ET' */
    Fortification,     /**< 'FT', capitals and villages. */
    Player,            /**< 'PL' */
    PlayerKnownSpells, /**< 'KS' */
    Fog,               /**< 'FG' */
    PlayerBuildings,   /**< 'PB' */
    Road,              /**< 'RA' */
    Stack,             /**< 'KC' */
    Unit,              /**< 'UN' */
    Landmark,          /**< 'MM' */
    Item,              /**< 'IM' */
    Bag,               /**< 'BG' */
    Site,              /**< 'SI' */
    Ruin,              /**< 'RU' */
    Tomb,              /**< 'TB' */
    Rod,               /**< 'RD' */
    Crystal,           /**< 'CR' */
    Diplomacy,         /**< 'DP' */
    SpellCast,         /**< 'ST' */
    Location,          /**< 'LO' */
    StackTemplate,     /**< 'TM' */
    Event,             /**< 'EV' */
    StackDestroyed,    /**< 'SD' */
    TalismanCharges,   /**< 'TC' */
    // 52 for MT
    Mountains = 53, /**< 'ML' */
    SubRace,        /**< 'SR' */
    // 55 for BR
    QuestLog = 56,    /**< 'QL' */
    TurnSummary,      /**< 'TS' */
    ScenarioVariable, /**< 'SV' */
    Invalid = 59
};

/**
 * Represents entity identifier in game.
 * Used in game logic as integer and stored as 10-characters string
 * in game databases and scenario files.
 * Id string examples: "S143MB0c08", "G000UU3001".
 * Both identifier representations contains 4 logical parts:
 * - IdCategory
 * - Category index
 * - IdType
 * - Type index
 * Example of id string "S143MB0c08" split into logical parts:
 * - 'S' - IdCategory (Scenario)
 * - '143' - Category index (scenario with index 143, values are decimal)
 * - 'MB' - IdType (Identifier describes map block entity)
 * - '0c08' - Type index (0x0c08, map block y and x coordinates on map)
 */
struct CMidgardID
{
    int value;
};

static_assert(sizeof(CMidgardID) == sizeof(int),
              "Size of CMidgardID structure must equal to the size of int");

static constexpr bool operator==(const CMidgardID& first, const CMidgardID& second)
{
    return first.value == second.value;
}

static constexpr bool operator!=(const CMidgardID& first, const CMidgardID& second)
{
    return !(first == second);
}

static constexpr bool operator<(const CMidgardID& first, const CMidgardID& second)
{
    return first.value < second.value;
}

extern const CMidgardID invalidId;
extern const CMidgardID emptyId;

namespace CMidgardIDApi {

struct Api
{
    /**
     * Returns category part from id.
     * Same as @code{.cpp}id->value >> 30;@endcode
     * @returns IdCategory::Invalid in case of invalid id.
     */
    using GetCategory = IdCategory(__thiscall*)(const CMidgardID* id);
    GetCategory getCategory;

    /**
     * Returns category index part from id.
     * Same as @code{.cpp}(id->value >> 22) & 0xff;@endcode
     * @returns invalid index of 256 in case of invalid id.
     */
    using GetCategoryIndex = int(__thiscall*)(const CMidgardID* id);
    GetCategoryIndex getCategoryIndex;

    /**
     * Returns type part from id.
     * Same as @code{.cpp}(id->value >> 16) & 0x3f;@endcode
     * @returns IdType::Invalid in case of invalid id.
     */
    using GetType = IdType(__thiscall*)(const CMidgardID* id);
    GetType getType;

    /**
     * Returns type index part from id.
     * Same as @code{.cpp}id->value & 0xffff;@endcode
     * @returns invalid index value of 0x10000 in case of invalid id.
     */
    using GetTypeIndex = int(__thiscall*)(const CMidgardID* id);
    GetTypeIndex getTypeIndex;

    /**
     * Converts id value to 10 characters length string.
     * Creates "INVALID-ID" string in case of error.
     */
    using ToString = char*(__thiscall*)(const CMidgardID* id, char* string);
    ToString toString;

    /**
     * Converts string to id value.
     * Creates invalid id value of 0x3f0000 in case of error.
     */
    using FromString = CMidgardID*(__thiscall*)(CMidgardID* id, const char* string);
    FromString fromString;

    /**
     * Creates id value from its parts.
     * @returns invalid id value in case of invalid parts.
     */
    using FromParts = CMidgardID*(__stdcall*)(CMidgardID* id,
                                              IdCategory category,
                                              int categoryIndex,
                                              IdType type,
                                              int typeIndex);
    FromParts fromParts;

    /** Returns true if string can be converted to valid id. */
    using IsIdStringValid = bool(__stdcall*)(const char* string);
    IsIdStringValid isIdStringValid;

    /**
     * Checks if 'other' is a special id for summoning units in battle.
     * 'other' id can be a valid unit id or special id like "G000004e2n",
     * where n is a position in group for upcoming summon.
     * Sets 'id' to emptyId if 'other' is a valid unit id, otherwise copies 'other' into 'id'.
     * @param[inout] id id to store result.
     * @param[in] other id to check.
     * @returns pointer to 'id'.
     */
    using IsSummonUnitId = CMidgardID*(__thiscall*)(CMidgardID* id, const CMidgardID* other);
    IsSummonUnitId isSummonUnitId;

    /**
     * Creates special id for summoning units in battle from specified position in group.
     * 'id' isn't changed if position is not in valid range [0 : 5].
     * @param[inout] id to store result.
     * @param position unit position in group.
     */
    using SummonUnitIdFromPosition = void(__thiscall*)(CMidgardID* id, int position);
    SummonUnitIdFromPosition summonUnitIdFromPosition;

    /**
     * Returns position from special id for summoning units in battle.
     * Does not check if unit special or not.
     * Same as @code{.cpp}getTypeIndex(id) - 0x4e20;@endcode
     */
    using SummonUnitIdToPosition = int(__thiscall*)(const CMidgardID* id);
    SummonUnitIdToPosition summonUnitIdToPosition;

    /** Creates a new id from the specified id and type. */
    using ChangeType = CMidgardID*(__stdcall*)(CMidgardID* id,
                                               const CMidgardID* src,
                                               IdType newType);
    ChangeType changeType;

    /** Checks if all parts of the id are correct and copies it to value. Otherwise, returns the
     * invalidId.
     */
    using ValidateId = CMidgardID*(__stdcall*)(CMidgardID* value, CMidgardID id);
    ValidateId validateId;
};

Api& get();

} // namespace CMidgardIDApi

} // namespace game

#endif // MIDGARDID_H
