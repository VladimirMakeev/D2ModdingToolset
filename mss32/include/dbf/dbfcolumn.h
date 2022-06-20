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

#ifndef DBFCOLUMN_H
#define DBFCOLUMN_H

#include "d2assert.h"
#include <cstddef>
#include <cstdint>

namespace utils {

enum class ColumnType : char
{
    Character = 'C',
    Number = 'N',
    Logical = 'L'
};

struct DbfColumn
{
    char name[11];
    ColumnType type;

    std::uint32_t dataAddress; /**< Offset from the start of the record. */
    std::uint8_t length;       /**< Length of the data in bytes. */
    std::uint8_t decimalCount; /**< Number of digits after decimal point. */

    char multiUserReserved[2];

    std::uint8_t workAreaId;

    char multiUserReserved2[2];

    bool setFields;

    char reserved[7];

    bool indexField;
};

assert_size(DbfColumn, 32);
assert_offset(DbfColumn, dataAddress, 12);
assert_offset(DbfColumn, workAreaId, 20);

} // namespace utils

#endif // DBFCOLUMN_H
