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

#include "dbfrecord.h"
#include "dbffile.h"
#include <charconv>

namespace utils {

DbfRecord::DbfRecord(const DbfFile* dbf, RecordData recordData, bool deleted)
    : dbf(dbf)
    , data(recordData)
    , deleted(deleted)
{ }

bool DbfRecord::value(std::string& result, std::uint32_t columnIndex) const
{
    if (!dbf) {
        return false;
    }

    const auto column = dbf->column(columnIndex);
    if (!column) {
        return false;
    }

    return value(result, *column);
}

bool DbfRecord::value(std::string& result, const std::string& columnName) const
{
    if (!dbf) {
        return false;
    }

    const auto column = dbf->column(columnName);
    if (!column) {
        return false;
    }

    return value(result, *column);
}

bool DbfRecord::value(std::string& result, const DbfColumn& column) const
{
    if (data.empty()) {
        return false;
    }

    if (column.type != ColumnType::Character) {
        return false;
    }

    const char* bgn = reinterpret_cast<const char*>(&data[column.dataAddress]);
    result = std::string(bgn, column.length);
    return true;
}

bool DbfRecord::value(int& result, std::uint32_t columnIndex) const
{
    if (!dbf) {
        return false;
    }

    const auto column = dbf->column(columnIndex);
    if (!column) {
        return false;
    }

    return value(result, *column);
}

bool DbfRecord::value(int& result, const std::string& columnName) const
{
    if (!dbf) {
        return false;
    }

    const auto column = dbf->column(columnName);
    if (!column) {
        return false;
    }

    return value(result, *column);
}

bool DbfRecord::value(int& result, const DbfColumn& column) const
{
    if (data.empty()) {
        return false;
    }

    if (column.type != ColumnType::Number) {
        return false;
    }

    const char* first = reinterpret_cast<const char*>(&data[column.dataAddress]);
    auto length = column.length;
    // skip spaces at the start of the field to std::from_chars work properly
    while (*first == ' ' && length) {
        first++;
        length--;
    }

    if (!length) {
        return false;
    }

    const char* last = first + length;
    int tmpResult{};
    auto [p, ec] = std::from_chars(first, last, tmpResult);
    if (ec != std::errc()) {
        return false;
    }

    result = tmpResult;
    return true;
}

bool DbfRecord::value(bool& result, std::uint32_t columnIndex) const
{
    if (!dbf) {
        return false;
    }

    const auto column = dbf->column(columnIndex);
    if (!column) {
        return false;
    }

    return value(result, *column);
}

bool DbfRecord::value(bool& result, const std::string& columnName) const
{
    if (!dbf) {
        return false;
    }

    const auto column = dbf->column(columnName);
    if (!column) {
        return false;
    }

    return value(result, *column);
}

bool DbfRecord::value(bool& result, const DbfColumn& column) const
{
    if (data.empty()) {
        return false;
    }

    if (column.type != ColumnType::Logical) {
        return false;
    }

    result = data[column.dataAddress] == 'T';
    return true;
}

bool DbfRecord::isDeleted() const
{
    return deleted;
}

} // namespace utils
