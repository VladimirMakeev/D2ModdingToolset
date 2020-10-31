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

#ifndef DBFACCESS_H
#define DBFACCESS_H

#include <string>

namespace game {
struct CMidgardID;
}

namespace utils {

class DbfFile;

/**
 * Reads identifier from game database.
 * @param[inout] id identifier to store results.
 * @param[in] database game database to read from, must be opened before read.
 * @param row row number to access
 * @param[in] columnName database column name to access
 * @returns false in case of invalid row number or nonexisting column name.
 */
bool dbRead(game::CMidgardID& id,
            const DbfFile& database,
            size_t row,
            const std::string& columnName);

/**
 * Reads integer value from game database.
 * @param[inout] result variable to store results.
 * @param[in] database game database to read from, must be opened before read.
 * @param row row number to access
 * @param[in] columnName database column name to access
 * @returns false in case of invalid row number or nonexisting column name.
 */
bool dbRead(int& result, const DbfFile& database, size_t row, const std::string& columnName);

} // namespace utils

#endif // DBFACCESS_H
