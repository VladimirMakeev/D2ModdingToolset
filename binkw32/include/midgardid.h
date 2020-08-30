/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

/**
 * Represents entity identifier in game.
 * Used in game logic as integer and stored as 10-characters string
 * in game databases and scenario files.
 * Id string examples: "S143MB0c08", "G000UU3001".
 * Both identifier representations contains 4 logical parts:
 * - IdCategory
 * - Category index
 * - Type
 * - Type index
 * Example of id string "S143MB0c08" split into logical parts:
 * - 'S' - IdCategory (Scenario)
 * - '143' - Category index (scenario with index 143, values are decimal)
 * - 'MB' - Type (Identifier describes map block entity)
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
     * @returns invalid type value of 59 in case of invalid id.
     */
    using GetType = int(__thiscall*)(const CMidgardID* id);
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
                                              int type,
                                              int typeIndex);
    FromParts fromParts;

    /** Returns true if string can be converted to valid id. */
    using IsIdStringValid = bool(__stdcall*)(const char* string);
    IsIdStringValid isIdStringValid;
};

Api& get();

} // namespace CMidgardIDApi

} // namespace game

#endif // MIDGARDID_H
