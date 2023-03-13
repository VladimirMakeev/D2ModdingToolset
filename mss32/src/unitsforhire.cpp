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

#include "unitsforhire.h"
#include "categoryids.h"
#include "dbf/dbffile.h"
#include "dbfaccess.h"
#include "log.h"
#include "midgardid.h"
#include "utils.h"
#include <fmt/format.h>
#include <string>
#include <type_traits>

/** Converts enum value to underlying integral type. */
template <typename T>
static constexpr auto toIntegral(T enumValue)
{
    return static_cast<typename std::underlying_type<T>::type>(enumValue);
}

namespace hooks {

static UnitsForHire units;

bool loadUnitsForHire()
{
    using namespace utils;

    const std::string raceDbName{"Grace.dbf"};

    DbfFile raceDb;
    if (!raceDb.open(globalsFolder() / raceDbName)) {
        logError("mssProxyError.log", fmt::format("Could not read {:s} database.", raceDbName));
        return false;
    }

    // check how many new soldier_n columns we have, starting from soldier_6
    constexpr size_t columnsMax{10};
    size_t newColumns{};
    for (; newColumns < columnsMax; ++newColumns) {
        const DbfColumn* column{raceDb.column(fmt::format("SOLDIER_{:d}", newColumns + 6))};
        if (!column) {
            break;
        }
    }

    if (!newColumns) {
        return true;
    }

    UnitsForHire tmpUnits(raceDb.recordsTotal());

    for (size_t row = 0; row < raceDb.recordsTotal(); ++row) {
        const std::string idColumnName{"RACE_ID"};

        game::CMidgardID raceId{};
        if (!dbRead(raceId, raceDb, row, idColumnName)) {
            logError("mssProxyError.log",
                     fmt::format("Failed to read row {:d} column {:s} from {:s} database.", row,
                                 idColumnName, raceDbName));
            return false;
        }

        if (raceId == game::invalidId) {
            logError("mssProxyError.log",
                     fmt::format("Row {:d} has invalid {:s} value in {:s} database.", row,
                                 idColumnName, raceDbName));
            return false;
        }

        const auto& idApi = game::CMidgardIDApi::get();
        const int raceIndex = idApi.getTypeIndex(&raceId);
        if (raceIndex >= (int)tmpUnits.size()) {
            logError("mssProxyError.log", fmt::format("Row {:d} column {:s} has invalid "
                                                      "race index {:d} in {:s} database.",
                                                      row, idColumnName, raceIndex, raceDbName));
            return false;
        }

        // Neutrals race does not hire units, skip
        if (raceIndex == toIntegral(game::RaceId::Neutral)) {
            continue;
        }

        for (size_t i = 0; i < newColumns; ++i) {
            const std::string columnName{fmt::format("SOLDIER_{:d}", i + 6)};
            game::CMidgardID soldierId{};
            if (!dbRead(soldierId, raceDb, row, columnName) || soldierId == game::invalidId) {
                logError("mssProxyError.log",
                         fmt::format("Row {:d} column {:s} has invalid id in {:s} database", row,
                                     columnName, raceDbName));
                return false;
            }

            // Allow different races to have different number of units to hire
            if (soldierId == game::emptyId) {
                continue;
            }

            tmpUnits[raceIndex].push_back(soldierId);
        }
    }

    units.swap(tmpUnits);
    return true;
}

const UnitsForHire& unitsForHire()
{
    return units;
}

} // namespace hooks
