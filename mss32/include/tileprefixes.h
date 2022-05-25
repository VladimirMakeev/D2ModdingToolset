/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef TILEPREFIXES_H
#define TILEPREFIXES_H

#include <cstdint>

namespace game {

/** Used to convert between tile name prefixes and vice versa. */
enum class TilePrefix : std::uint32_t
{
    Water,   /**< WA */
    Neutral, /**< NE */
    Human,   /**< HU */
    Heretic, /**< HE */
    Dwarf,   /**< DW */
    Undead,  /**< UN */
    Black,   /**< BL */
    Elf,     /**< EL */
};

namespace TilePrefixApi {

struct Api
{
    /** Returns tile prefix number by tile prefix name. */
    using GetTilePrefixByName = TilePrefix(__stdcall*)(const char* tileNamePrefix);
    GetTilePrefixByName getTilePrefixByName;

    /** Returns tile prefix name by tile prefix number. */
    using GetTilePrefixName = const char*(__stdcall*)(TilePrefix tilePrefix);
    GetTilePrefixName getTilePrefixName;
};

Api& get();

} // namespace TilePrefixApi

} // namespace game

#endif // TILEPREFIXES_H
