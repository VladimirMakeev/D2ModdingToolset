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

#include "subracehooks.h"
#include "dbf/dbffile.h"
#include "log.h"
#include "subracecat.h"
#include "utils.h"
#include <algorithm>
#include <fmt/format.h>

namespace hooks {

static std::vector<std::pair<game::LSubRaceCategory, std::string>> newSubRaces;

static void checkNewSubRaceCategories(const std::filesystem::path& dbfFilePath)
{
    utils::DbfFile dbf;
    if (!dbf.open(dbfFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbfFilePath.filename().string()));
        return;
    }

    static const std::array<const char*, 15> knownCategories = {
        {"L_CUSTOM", "L_HUMAN", "L_UNDEAD", "L_HERETIC", "L_DWARF", "L_NEUTRAL", "L_NEUTRAL_HUMAN",
         "L_NEUTRAL_ELF", "L_NEUTRAL_GREEN_SKIN", "L_NEUTRAL_DRAGON", "L_NEUTRAL_MARSH",
         "L_NEUTRAL_WATER", "L_NEUTRAL_BARBARIAN", "L_NEUTRAL_WOLF", "L_ELF"}};

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
            logDebug("newRace.log", fmt::format("Found new subrace category {:s}", categoryName));

            newSubRaces.push_back({game::LSubRaceCategory{}, categoryName});
        }
    }
}

game::LSubRaceCategoryTable* __fastcall subRaceCategoryTableCtorHooked(
    game::LSubRaceCategoryTable* thisptr,
    int /*%edx*/,
    const char* globalsFolderPath,
    void* codeBaseEnvProxy)
{
    using namespace game;

    logDebug("newRace.log", "LSubRaceCategoryTable c-tor hook started");

    static const char dbfFileName[] = "LSubRace.dbf";
    checkNewSubRaceCategories(std::filesystem::path(globalsFolderPath) / dbfFileName);

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LSubRaceCategoryTableApi::vftable();

    const auto& table = LSubRaceCategoryTableApi::get();
    auto& subraces = SubraceCategories::get();

    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);
    table.readCategory(subraces.custom, thisptr, "L_CUSTOM", dbfFileName);
    table.readCategory(subraces.human, thisptr, "L_HUMAN", dbfFileName);
    table.readCategory(subraces.dwarf, thisptr, "L_DWARF", dbfFileName);

    table.readCategory(subraces.heretic, thisptr, "L_HERETIC", dbfFileName);
    table.readCategory(subraces.undead, thisptr, "L_UNDEAD", dbfFileName);
    table.readCategory(subraces.neutral, thisptr, "L_NEUTRAL", dbfFileName);

    table.readCategory(subraces.neutralHuman, thisptr, "L_NEUTRAL_HUMAN", dbfFileName);
    table.readCategory(subraces.neutralElf, thisptr, "L_NEUTRAL_ELF", dbfFileName);
    table.readCategory(subraces.neutralGreenSkin, thisptr, "L_NEUTRAL_GREEN_SKIN", dbfFileName);

    table.readCategory(subraces.neutralDragon, thisptr, "L_NEUTRAL_DRAGON", dbfFileName);
    table.readCategory(subraces.neutralMarsh, thisptr, "L_NEUTRAL_MARSH", dbfFileName);
    table.readCategory(subraces.neutralWater, thisptr, "L_NEUTRAL_WATER", dbfFileName);

    table.readCategory(subraces.neutralBarbarian, thisptr, "L_NEUTRAL_BARBARIAN", dbfFileName);
    table.readCategory(subraces.neutralWolf, thisptr, "L_NEUTRAL_WOLF", dbfFileName);
    table.readCategory(subraces.elf, thisptr, "L_NEUTRAL_WOLF", dbfFileName);

    for (auto& subrace : newSubRaces) {
        logDebug("newRace.log", fmt::format("Read new subrace category {:s}", subrace.second));
        table.readCategory(&subrace.first, thisptr, subrace.second.c_str(), dbfFileName);
    }

    table.initDone(thisptr);

    logDebug("newRace.log", "LSubRaceCategoryTable c-tor hook finished");

    return thisptr;
}

} // namespace hooks
