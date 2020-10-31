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

#include "dbffile.h"
#include <cassert>

namespace utils {

bool DbfFile::open(const std::filesystem::path& file)
{
    valid = false;

    std::ifstream stream(file, std::ios_base::binary);
    if (!stream.is_open()) {
        return false;
    }

    stream.seekg(0, stream.end);
    const auto fileSize = stream.tellg();
    stream.seekg(0, stream.beg);

    if (fileSize < sizeof(DbfHeader)) {
        return false;
    }

    if (!readHeader(stream)) {
        return false;
    }

    const auto recordsDataLength = recordsTotal() * header.recordLength;
    const auto recordsEnd = header.headerLength + recordsDataLength;
    if (recordsEnd + 1 != fileSize) {
        return false;
    }

    if (!readColumns(stream)) {
        return false;
    }

    char endOfFields{};
    stream.read(&endOfFields, 1);

    if (endOfFields != 0xd) {
        return false;
    }

    recordsData.resize(recordsDataLength);
    stream.read(reinterpret_cast<char*>(&recordsData[0]), recordsDataLength);

    valid = true;
    return true;
}

CodePage DbfFile::language() const
{
    return header.language;
}

std::uint32_t DbfFile::columnsTotal() const
{
    return (std::uint32_t)columns.size();
}

std::uint32_t DbfFile::recordsTotal() const
{
    return header.recordsTotal;
}

const DbfColumn* DbfFile::column(std::uint32_t index) const
{
    return index < columns.size() ? &columns[index] : nullptr;
}

const DbfColumn* DbfFile::column(const std::string& name) const
{
    const auto it = columnIndices.find(name);
    if (it == columnIndices.end()) {
        return nullptr;
    }

    return column(it->second);
}

bool DbfFile::record(DbfRecord& result, std::uint32_t index) const
{
    if (index >= recordsTotal()) {
        return false;
    }

    const auto* bgn = &recordsData[index * header.recordLength];
    bool deleted = *bgn == 0x2a;
    result = DbfRecord(this, DbfRecord::RecordData(bgn + 1, header.recordLength), deleted);
    return true;
}

bool DbfFile::readHeader(std::ifstream& stream)
{
    DbfHeader tmpHeader;
    stream.read(reinterpret_cast<char*>(&tmpHeader), sizeof(tmpHeader));

    if (tmpHeader.version.parts.version != 0x3) {
        return false;
    }

    if (tmpHeader.headerLength == 0 || tmpHeader.headerLength < sizeof(DbfHeader) + 1) {
        return false;
    }

    if (tmpHeader.recordLength == 0) {
        return false;
    }

    header = tmpHeader;
    return true;
}

bool DbfFile::readColumns(std::ifstream& stream)
{
    const auto columnsTotal = (header.headerLength - sizeof(DbfHeader) - 1) / sizeof(DbfColumn);
    Columns tmpColumns(columnsTotal);
    ColumnIndexMap tmpIndices;

    std::uint32_t index{0};
    std::uint32_t dataAddress{0};
    for (auto& column : tmpColumns) {
        if (!readColumn(stream, column)) {
            return false;
        }

        column.dataAddress = dataAddress;
        dataAddress += column.length;
        tmpIndices[column.name] = index++;
    }

    columns.swap(tmpColumns);
    columnIndices.swap(tmpIndices);
    return true;
}

bool DbfFile::readColumn(std::ifstream& stream, DbfColumn& column)
{
    stream.read(reinterpret_cast<char*>(&column), sizeof(DbfColumn));
    return true;
}

} // namespace utils
