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

#include "isoviewhooks.h"
#include "gameutils.h"
#include "groundcat.h"
#include "image2text.h"
#include "isoview.h"
#include "midgardmap.h"
#include "midgardscenariomap.h"
#include "scenedit.h"
#include "terraincat.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

static const char* groundToString(const game::LGroundCategory& ground)
{
    using namespace game;

    const auto& categories = GroundCategories::get();
    if (ground.id == categories.plain->id) {
        return "Plains";
    }

    if (ground.id == categories.water->id) {
        return "Water";
    }

    if (ground.id == categories.forest->id) {
        return "Forest";
    }

    if (ground.id == categories.mountain->id) {
        return "Mountain";
    }

    return "";
}

static const char* terrainToString(const game::LTerrainCategory& terrain)
{
    using namespace game;

    const auto& categories = TerrainCategories::get();
    if (terrain.id == categories.neutral->id) {
        return "Neutral";
    }

    if (terrain.id == categories.human->id) {
        return "Empire";
    }

    if (terrain.id == categories.dwarf->id) {
        return "Mountain Clans";
    }

    if (terrain.id == categories.heretic->id) {
        return "Legions of the Damned";
    }

    if (terrain.id == categories.undead->id) {
        return "Undead Hordes";
    }

    if (terrain.id == categories.elf->id) {
        return "Elven Alliance";
    }

    return "";
}

void __fastcall updateSelectedTileInfoHooked(game::editor::CIsoView* thisptr,
                                             int /* %edx */,
                                             const game::CMqPoint* mapPosition)
{
    using namespace game;

    const CScenEdit* scenEdit = CScenEditApi::get().instance();
    const IMidgardObjectMap* objectMap = scenEdit->data->unknown2->data->scenarioMap;

    const CMidgardMap* map = getMidgardMap(objectMap);
    const auto& mapApi = CMidgardMapApi::get();

    LTerrainCategory terrain{};
    if (!mapApi.getTerrain(map, &terrain, mapPosition, objectMap)) {
        return;
    }

    LGroundCategory ground{};
    if (!mapApi.getGround(map, &ground, mapPosition, objectMap)) {
        return;
    }

    // \c000;255;000;%X%:%Y%\n%GROUND%\n%TERRAIN%
    std::string info = getInterfaceText("X100TA0774");

    replace(info, "%X%", std::to_string(mapPosition->x));
    replace(info, "%Y%", std::to_string(mapPosition->y));
    replace(info, "%GROUND%", groundToString(ground));
    replace(info, "%TERRAIN%", terrainToString(terrain));

    CImage2TextApi::get().setText(thisptr->isoViewData->selectedTileInfo, info.c_str());
}

} // namespace hooks
