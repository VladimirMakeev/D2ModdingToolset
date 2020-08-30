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

#include "dbfaccess.h"
#include "dbf/dbffile.h"
#include "midgardid.h"
#include <functional>

namespace utils {

template <typename T>
using Convertor = std::function<bool(T&, const DbfRecord&, const DbfColumn&)>;

template <typename T>
bool dbRead(T& result,
            const DbfFile& database,
            size_t row,
            const std::string& columnName,
            const Convertor<T>& convertor)
{
    if (row >= database.recordsTotal()) {
        return false;
    }

    DbfRecord record;
    if (!database.record(record, row)) {
        return false;
    }

    const DbfColumn* column{database.column(columnName)};
    if (!column) {
        return false;
    }

    return convertor(result, record, *column);
}

bool dbRead(game::CMidgardID& id,
            const DbfFile& database,
            size_t row,
            const std::string& columnName)
{
    auto convertId = [](game::CMidgardID& id, const DbfRecord& record, const DbfColumn& column) {
        std::string idString;
        if (!record.value(idString, column)) {
            return false;
        }

        const auto& idApi = game::CMidgardIDApi::get();
        game::CMidgardID tmpId{};
        idApi.fromString(&tmpId, idString.c_str());
        if (tmpId == game::invalidId) {
            return false;
        }

        id = tmpId;
        return true;
    };

    return dbRead<game::CMidgardID>(id, database, row, columnName, convertId);
}

bool dbRead(int& result, const DbfFile& database, size_t row, const std::string& columnName)
{
    auto convertInt = [](int& result, const DbfRecord& record, const DbfColumn& column) {
        int tmp{};
        if (!record.value(tmp, column)) {
            return false;
        }

        result = tmp;
        return true;
    };

    return dbRead<int>(result, database, row, columnName, convertInt);
}

} // namespace utils
