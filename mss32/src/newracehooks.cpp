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

#include "newracehooks.h"
#include "dbf/dbffile.h"
#include "game.h"
#include "log.h"
#include "racecategory.h"
#include "racetype.h"
#include "utils.h"
#include <algorithm>
#include <fmt/format.h>

namespace hooks {

static std::vector<std::pair<game::LRaceCategory, std::string>> newRaces;

static void checkNewRaceCategories(const std::filesystem::path& dbfFilePath)
{
    utils::DbfFile dbf;
    if (!dbf.open(dbfFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbfFilePath.filename().string()));
        return;
    }

    static const std::array<const char*, 6> knownCategories = {
        {"L_HUMAN", "L_UNDEAD", "L_HERETIC", "L_DWARF", "L_NEUTRAL", "L_ELF"}};

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
            logDebug("newRace.log", fmt::format("Found new race category {:s}", categoryName));

            newRaces.push_back({game::LRaceCategory{}, categoryName});
        }
    }
}

game::LRaceCategoryTable* __fastcall raceCategoryTableCtorHooked(game::LRaceCategoryTable* thisptr,
                                                                 int /*%edx*/,
                                                                 const char* globalsFolderPath,
                                                                 void* codeBaseEnvProxy)
{
    using namespace game;

    logDebug("newRace.log", "LRaceCategoryTable c-tor hook started");

    static const char dbfFileName[] = "LRace.dbf";
    checkNewRaceCategories(std::filesystem::path(globalsFolderPath) / dbfFileName);

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LRaceCategoryTableApi::vftable();

    const auto& table = LRaceCategoryTableApi::get();
    auto& races = RaceCategories::get();

    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);
    table.readCategory(races.human, thisptr, "L_HUMAN", dbfFileName);
    table.readCategory(races.dwarf, thisptr, "L_DWARF", dbfFileName);
    table.readCategory(races.heretic, thisptr, "L_HERETIC", dbfFileName);
    table.readCategory(races.undead, thisptr, "L_UNDEAD", dbfFileName);
    table.readCategory(races.neutral, thisptr, "L_NEUTRAL", dbfFileName);
    table.readCategory(races.elf, thisptr, "L_ELF", dbfFileName);

    for (auto& race : newRaces) {
        logDebug("newRace.log", fmt::format("Read new race category {:s}", race.second));
        table.readCategory(&race.first, thisptr, race.second.c_str(), dbfFileName);
    }

    table.initDone(thisptr);

    logDebug("newRace.log", "LRaceCategoryTable c-tor hook finished");
    return thisptr;
}

void __fastcall validateRacesHooked(game::RacesMap** thisptr,
                                    int /*%edx*/,
                                    game::GlobalData** globalData)
{
    using namespace game;

    logDebug("newRace.log", "Validate races hook started");

    auto racesMap = *thisptr;
    auto current = racesMap->data.bgn;
    auto end = racesMap->data.end;

    while (current != end) {
        auto race = current->second;
        auto validateRace = static_cast<const TRaceTypeVftable*>(race->vftable)->validate;
        validateRace(race, globalData);
        current++;
    }

    static const char dbfFileName[] = "Grace.dbf";
    const auto& fn = gameFunctions();
    auto& races = RaceCategories::get();

    fn.checkRaceExist(thisptr, races.human, dbfFileName);
    fn.checkRaceExist(thisptr, races.dwarf, dbfFileName);
    fn.checkRaceExist(thisptr, races.heretic, dbfFileName);
    fn.checkRaceExist(thisptr, races.undead, dbfFileName);
    fn.checkRaceExist(thisptr, races.elf, dbfFileName);

    for (auto& race : newRaces) {
        fn.checkRaceExist(thisptr, &race.first, dbfFileName);
    }

    logDebug("newRace.log", "Validate races hook finished");
}

} // namespace hooks
