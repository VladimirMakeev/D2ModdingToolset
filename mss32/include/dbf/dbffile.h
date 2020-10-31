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

#ifndef DBFFILE_H
#define DBFFILE_H

#include "dbfcolumn.h"
#include "dbfheader.h"
#include "dbfrecord.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace utils {

class DbfFile
{
public:
    DbfFile() = default;

    bool open(const std::filesystem::path& file);
    bool isValid() const
    {
        return valid;
    }

    CodePage language() const;

    std::uint32_t columnsTotal() const;
    std::uint32_t recordsTotal() const;

    /** Returns nullptr in case of invalid index. */
    const DbfColumn* column(std::uint32_t index) const;
    /** Returns nullptr if column with specified name can not be found. */
    const DbfColumn* column(const std::string& name) const;

    /**
     * Creates thin wrapper for record data access.
     * Created records must not outlive DbfFile object that created them.
     * @returns false in case of invalid index.
     */
    bool record(DbfRecord& result, std::uint32_t index) const;

private:
    bool readHeader(std::ifstream& stream);
    bool readColumns(std::ifstream& stream);
    bool readColumn(std::ifstream& stream, DbfColumn& column);

    using Columns = std::vector<DbfColumn>;
    using ColumnIndexMap = std::unordered_map<std::string, std::uint32_t>;

    DbfHeader header{};
    Columns columns;
    ColumnIndexMap columnIndices;
    std::vector<std::uint8_t> recordsData;
    bool valid{};
};

} // namespace utils

#endif // DBFFILE_H
