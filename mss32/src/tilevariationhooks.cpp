/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#include "tilevariationhooks.h"
#include "game.h"
#include "log.h"
#include "terrainhooks.h"
#include "tilevariation.h"
#include <fmt/format.h>
#include <string>

namespace hooks {

static bool checkNeutralTileVariation(const game::CTileVariationData* data, std::string& message)
{
    using namespace game;

    const auto& checkRecordsCorrect = CTileVariationApi::get().checkRecordsCorrect;
    const auto& terrain = TerrainCategories::get();
    const auto& ground = GroundCategories::get();

    if (!checkRecordsCorrect(data, terrain.neutral, ground.plain, 82)) {
        message = fmt::format("TileVariation: Missing tiles for Neutral, Plains");
        return false;
    }

    if (!checkRecordsCorrect(data, terrain.neutral, ground.forest, 1)) {
        message = fmt::format("TileVariation: Missing tiles for Neutral, Forest");
        return false;
    }

    if (!checkRecordsCorrect(data, terrain.neutral, ground.water, 82)) {
        message = fmt::format("TileVariation: Missing tiles for Neutral, Water");
        return false;
    }

    if (!checkRecordsCorrect(data, terrain.neutral, ground.mountain, 6)) {
        message = fmt::format("TileVariation: Missing tiles for Neutral, Mountains");
        return false;
    }

    return true;
}

static bool checkRaceTileVariation(const game::CTileVariationData* data,
                                   const game::LTerrainCategory* terrain,
                                   const char* raceName,
                                   std::string& message)
{
    using namespace game;

    const auto& checkRecordsCorrect = CTileVariationApi::get().checkRecordsCorrect;
    const auto& ground = GroundCategories::get();

    if (!checkRecordsCorrect(data, terrain, ground.plain, 82)) {
        message = fmt::format("TileVariation: Missing tiles for {:s}, Plains", raceName);
        return false;
    }

    if (!checkRecordsCorrect(data, terrain, ground.forest, 1)) {
        message = fmt::format("TileVariation: Missing tiles for {:s}, Forest", raceName);
        return false;
    }

    if (!checkRecordsCorrect(data, terrain, ground.mountain, 6)) {
        message = fmt::format("TileVariation: Missing tiles for {:s}, Mountains", raceName);
        return false;
    }

    return true;
}

void __stdcall checkTileVariationDataHooked(const game::CTileVariationData* data)
{
    using namespace game;

    logDebug("newTerrain.log", "Check CTileVariationData hook started");

    const auto& terrains = TerrainCategories::get();
    std::string message;
    bool error{false};

    if (!checkNeutralTileVariation(data, message)
        || !checkRaceTileVariation(data, terrains.dwarf, "Dwarf", message)
        || !checkRaceTileVariation(data, terrains.heretic, "Heretic", message)
        || !checkRaceTileVariation(data, terrains.human, "Human", message)
        || !checkRaceTileVariation(data, terrains.undead, "Undead", message)
        || !checkRaceTileVariation(data, terrains.elf, "Elf", message)) {
        error = true;
    }

    for (const auto& terrain : customTerrains()) {
        if (!checkRaceTileVariation(data, &terrain.category, terrain.categoryName.c_str(),
                                    message)) {
            error = true;
            break;
        }
    }

    if (error) {
        gameFunctions().throwGenericException(message.c_str());
    }

    logDebug("newTerrain.log", "Check CTileVariationData hook finished");
}

} // namespace hooks
