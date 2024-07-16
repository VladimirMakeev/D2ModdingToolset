/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#include "scenedithooks.h"
#include "originalfunctions.h"
#include "dbf/dbffile.h"
#include "utils.h"
#include "game.h"
#include <fmt/format.h>

namespace hooks {

static MarketNames marketNames;

static bool readMarketNames(const std::filesystem::path& dbPath)
{
    utils::DbfFile db;
    if (!db.open(dbPath)) {
        return false;
    }

    const std::uint32_t total{db.recordsTotal()};
    marketNames.reserve(total);

    const auto& oemToChar = *game::gameFunctions().oemToCharA;

    for (std::uint32_t i = 0u; i < total; ++i) {
        utils::DbfRecord record;
        if (!db.record(record, i)) {
            return false;
        }

        if (record.isDeleted()) {
            continue;
        }

        std::string name;
        if (!record.value(name, "NAME")) {
            return false;
        }

        oemToChar(name.c_str(), name.data());

        std::string description;
        if (!record.value(description, "DESC")) {
            return false;
        }

        oemToChar(description.c_str(), description.data());

        marketNames.emplace_back(NameDescPair{trimSpaces(name), trimSpaces(description)});
    }

    return true;
}

bool __fastcall readScenDataHooked(game::CScenEdit* thisptr, int /*%edx*/)
{
	using namespace game;

	const auto dbPath{scenDataFolder() / "Marketname.dbf"};
    if (std::filesystem::exists(dbPath)) {
        if (!readMarketNames(dbPath)) {
            showErrorMessageBox(
                fmt::format("Failed to read resource market names from '{:s}'", dbPath.string()));
        }
	}

	return getOriginalFunctions().readScenData(thisptr);
}

const MarketNames& getMarketNames()
{
    return marketNames;
}

}