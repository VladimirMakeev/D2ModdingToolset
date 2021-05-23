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

#include "terrainhooks.h"
#include "dbf/dbffile.h"
#include "groundcat.h"
#include "log.h"
#include "originalfunctions.h"
#include "racecategory.h"
#include "racehooks.h"
#include "utils.h"
#include <algorithm>
#include <fmt/format.h>

namespace hooks {

static constexpr int customTilePrefixStartNumber{8};

static TerrainDescriptions customTerrainsData;

static void checkNewTerrainCategories(const std::filesystem::path& dbfFilePath)
{
    utils::DbfFile dbf;
    if (!dbf.open(dbfFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbfFilePath.filename().string()));
        return;
    }

    static const std::array<const char*, 6> knownCategories = {
        {"L_HU", "L_UN", "L_HE", "L_DW", "L_NE", "L_EL"}};

    const auto recordsTotal{dbf.recordsTotal()};
    for (std::uint32_t i = 0; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!dbf.record(record, i)) {
            logError("mssProxyError.log", fmt::format("Could not read record {:d} from {:s}", i,
                                                      dbfFilePath.filename().string()));
            return;
        }

        if (record.isDeleted()) {
            continue;
        }

        std::string categoryName;
        record.value(categoryName, "TEXT");
        categoryName = trimSpaces(categoryName);

        if (std::none_of(std::begin(knownCategories), std::end(knownCategories),
                         [&categoryName](const char* name) { return categoryName == name; })) {
            std::string abbrev{categoryName.substr(2, 2)};
            logDebug("newTerrain.log",
                     fmt::format("Found custom terrain category {:s}, abbrev {:s}", categoryName,
                                 abbrev));

            customTerrainsData.push_back({game::LTerrainCategory{}, categoryName, abbrev});
        }
    }
}

TerrainDescriptions& customTerrains()
{
    return customTerrainsData;
}

game::LTerrainCategoryTable* __fastcall terrainCategoryTableCtorHooked(
    game::LTerrainCategoryTable* thisptr,
    int /*%edx*/,
    const char* globalsFolderPath,
    void* codeBaseEnvProxy)
{
    using namespace game;

    logDebug("newTerrain.log", "LTerrainCategoryTable c-tor hook started");

    static const char dbfFileName[] = "LTerrain.dbf";
    checkNewTerrainCategories(std::filesystem::path(globalsFolderPath) / dbfFileName);

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LTerrainCategoryTableApi::vftable();

    const auto& table = LTerrainCategoryTableApi::get();
    auto& terrains = TerrainCategories::get();

    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);
    table.readCategory(terrains.human, thisptr, "L_HU", dbfFileName);
    table.readCategory(terrains.dwarf, thisptr, "L_DW", dbfFileName);
    table.readCategory(terrains.heretic, thisptr, "L_HE", dbfFileName);
    table.readCategory(terrains.undead, thisptr, "L_UN", dbfFileName);
    table.readCategory(terrains.neutral, thisptr, "L_NE", dbfFileName);
    table.readCategory(terrains.elf, thisptr, "L_EL", dbfFileName);

    for (auto& terrain : customTerrains()) {
        logDebug("newTerrain.log",
                 fmt::format("Read custom terrain category {:s}", terrain.categoryName));
        table.readCategory(&terrain.category, thisptr, terrain.categoryName.c_str(), dbfFileName);
    }

    table.initDone(thisptr);

    logDebug("newTerrain.log", "LTerrainCategoryTable c-tor hook finished");
    return thisptr;
}

game::LTerrainCategory* __stdcall getTerrainByAbbreviationHooked(game::LTerrainCategory* category,
                                                                 const char* abbreviation)
{
    getOriginalFunctions().getTerrainByAbbreviation(category, abbreviation);

    if (category->table == nullptr || static_cast<int>(category->id) == -1) {
        std::string name{abbreviation};

        std::transform(name.begin(), name.end(), name.begin(),
                       [](unsigned char c) { return std::toupper(c); });

        for (const auto& terrain : customTerrains()) {
            if (terrain.abbreviation == name) {
                category->id = terrain.category.id;
                category->table = terrain.category.table;
                break;
            }
        }
    }

    return category;
}

const game::LTerrainCategory* __stdcall getTerrainByRaceHooked(const game::LRaceCategory* race)
{
    using namespace game;

    const auto& races = RaceCategories::get();
    const auto& terrains = TerrainCategories::get();

    const auto raceId{race->id};

    if (raceId == races.human->id) {
        return terrains.human;
    }

    if (raceId == races.dwarf->id) {
        return terrains.dwarf;
    }

    if (raceId == races.heretic->id) {
        return terrains.heretic;
    }

    if (raceId == races.undead->id) {
        return terrains.undead;
    }

    if (raceId == races.elf->id) {
        return terrains.elf;
    }

    if (raceId == races.neutral->id) {
        return terrains.neutral;
    }

    // TODO: allow users to specify mapping in lua script
    for (size_t i = 0; i < newRaces().size(); ++i) {
        const auto& race = newRaces()[i];
        const auto& terrain = customTerrains()[i];

        if (race.abbreviation == terrain.abbreviation) {
            logDebug("newTerrain.log", fmt::format("Map {:s} race to {:s} terrain",
                                                   race.abbreviation, terrain.abbreviation));
            return &terrain.category;
        }
    }

    return terrains.neutral;
}

const game::LRaceCategory* __stdcall getRaceByTerrainHooked(const game::LTerrainCategory* terrain)
{
    using namespace game;

    const auto& races = RaceCategories::get();
    const auto& terrains = TerrainCategories::get();
    const auto terrainId{terrain->id};

    if (terrainId == terrains.human->id) {
        return races.human;
    }

    if (terrainId == terrains.heretic->id) {
        return races.heretic;
    }

    if (terrainId == terrains.undead->id) {
        return races.undead;
    }

    if (terrainId == terrains.dwarf->id) {
        return races.dwarf;
    }

    if (terrainId == terrains.elf->id) {
        return races.elf;
    }

    if (terrainId == terrains.neutral->id) {
        return races.neutral;
    }

    for (size_t i = 0; i < customTerrains().size(); ++i) {
        const auto& customTerrain = customTerrains()[i];

        if (terrainId == customTerrain.category.id) {
            return &newRaces()[i].category;
        }
    }

    logDebug("newTerrain.log", "Could not get race by terrain, assume neutrals");
    return races.neutral;
}

const game::LRaceCategory* __stdcall getPlayableRaceByTerrainHooked(
    const game::LTerrainCategory* terrain)
{
    using namespace game;

    const auto& races = RaceCategories::get();
    const auto& terrains = TerrainCategories::get();

    const auto terrainId{terrain->id};

    if (terrainId == terrains.human->id) {
        return races.human;
    }

    if (terrainId == terrains.heretic->id) {
        return races.heretic;
    }

    if (terrainId == terrains.undead->id) {
        return races.undead;
    }

    if (terrainId == terrains.dwarf->id) {
        return races.dwarf;
    }

    if (terrainId == terrains.elf->id) {
        return races.elf;
    }

    for (size_t i = 0; i < customTerrains().size(); ++i) {
        const auto& customTerrain = customTerrains()[i];

        if (terrainId == customTerrain.category.id) {
            return &newRaces()[i].category;
        }
    }

    logDebug("newTerrain.log", "Could not get playable race by terrain");
    return nullptr;
}

int __stdcall getTilePrefixByNameHooked(const char* tileNamePrefix)
{
    int customTilePrefix{customTilePrefixStartNumber};

    for (const auto& terrain : customTerrains()) {
        if (!_strcmpi(terrain.abbreviation.c_str(), tileNamePrefix)) {
            return customTilePrefix;
        }

        customTilePrefix++;
    }

    return getOriginalFunctions().getTilePrefixByName(tileNamePrefix);
}

const char* __stdcall getTilePrefixNameHooked(int tilePrefixNumber)
{
    if (tilePrefixNumber >= customTilePrefixStartNumber) {
        const auto index{tilePrefixNumber - customTilePrefixStartNumber};

        if (customTerrains().size() <= index) {
            return nullptr;
        }

        return customTerrains()[index].abbreviation.c_str();
    }

    return getOriginalFunctions().getTilePrefixName(tilePrefixNumber);
}

int __stdcall getNumberByTerrainGroundHooked(const game::LTerrainCategory* terrain,
                                             const game::LGroundCategory* ground)
{
    using namespace game;

    const auto& grounds = GroundCategories::get();

    if (ground->id == grounds.water->id) {
        return 6;
    }

    if (ground->id == grounds.mountain->id) {
        return 8;
    }

    const auto& terrains = TerrainCategories::get();

    if (terrain->id == terrains.heretic->id) {
        return 5;
    }

    if (terrain->id == terrains.human->id) {
        return 1;
    }

    if (terrain->id == terrains.dwarf->id) {
        return 2;
    }

    if (terrain->id == terrains.undead->id) {
        return 3;
    }

    if (terrain->id == terrains.elf->id) {
        return 4;
    }

    for (const auto& customTerrain : customTerrains()) {
        if (customTerrain.category.id == terrain->id) {
            // logDebug("newTerrain.log", "GetNumberByTerrainGround hook, return 4 for custom
            // terrain");
            return 4;
        }
    }

    return 0;
}

} // namespace hooks
