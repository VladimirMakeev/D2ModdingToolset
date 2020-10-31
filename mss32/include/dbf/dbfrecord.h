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

#ifndef DBFRECORD_H
#define DBFRECORD_H

#include "dbfcolumn.h"
#include <cstdint>
#include <gsl/span>

namespace utils {

class DbfFile;

class DbfRecord
{
public:
    using RecordData = gsl::span<const std::uint8_t>;

    DbfRecord() = default;
    DbfRecord(const DbfFile* dbf, RecordData recordData, bool deleted);

    // Characters fields access
    bool value(std::string& result, std::uint32_t columnIndex) const;
    bool value(std::string& result, const std::string& columnName) const;
    bool value(std::string& result, const DbfColumn& column) const;

    // Numeric fields access
    bool value(int& result, std::uint32_t columnIndex) const;
    bool value(int& result, const std::string& columnName) const;
    bool value(int& result, const DbfColumn& column) const;

    // Logical fields access
    bool value(bool& result, std::uint32_t columnIndex) const;
    bool value(bool& result, const std::string& columnName) const;
    bool value(bool& result, const DbfColumn& column) const;

private:
    const DbfFile* dbf{};
    RecordData data{};
    bool deleted{};
};

} // namespace utils

#endif // DBFRECORD_H
