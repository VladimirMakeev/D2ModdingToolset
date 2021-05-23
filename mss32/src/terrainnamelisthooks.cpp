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

#include "terrainnamelisthooks.h"
#include "log.h"
#include "originalfunctions.h"
#include "terrainhooks.h"
#include <fmt/format.h>

namespace hooks {

game::TerrainNameList* getTerrainNameListHooked()
{
    auto terrainNameList = getOriginalFunctions().getTerrainNameList();

    static bool firstTime{true};
    if (firstTime) {
        logDebug("newTerrain.log", "Add new terrain to terrain name list on first call");

        const auto& list = game::TerrainNameListApi::get();

        game::TerrainNameListIterator iterator{};
        for (const auto& terrain : customTerrains()) {
            logDebug("newTerrain.log", fmt::format("Add {:s} terrain", terrain.abbreviation));
            list.add(terrainNameList, &iterator, &terrain.category, terrain.abbreviation.c_str());
        }

        firstTime = false;
    }

    return terrainNameList;
}

} // namespace hooks
