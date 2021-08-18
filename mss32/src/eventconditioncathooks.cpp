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

#include "eventconditioncathooks.h"
#include "dbf/dbffile.h"
#include "log.h"
#include "midgardid.h"
#include "utils.h"
#include <algorithm>
#include <array>
#include <fmt/format.h>

namespace hooks {

static const char* ownResourceCategoryName{"L_OWN_RESOURCE"};
static const char* gameModeCategoryName{"L_GAME_MODE"};

CustomEventConditions& customEventConditions()
{
    static CustomEventConditions customConditions{};

    return customConditions;
}

static void readCustomCondition(const utils::DbfRecord& record, CustomEventCondition& condition)
{
    const auto& idApi = game::CMidgardIDApi::get();

    std::string info;
    record.value(info, "INFO");
    idApi.fromString(&condition.infoText, info.c_str());

    std::string brief;
    record.value(brief, "BRIEF");
    idApi.fromString(&condition.brief, brief.c_str());

    std::string descr;
    record.value(descr, "DESCR");
    idApi.fromString(&condition.description, descr.c_str());
}

static bool readCustomConditions(const std::filesystem::path& dbfFilePath)
{
    utils::DbfFile dbf;
    if (!dbf.open(dbfFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbfFilePath.filename().string()));
        return false;
    }

    bool customConditions{false};

    const auto recordsTotal{dbf.recordsTotal()};
    for (std::uint32_t i = 0; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!dbf.record(record, i)) {
            logError("mssProxyError.log", fmt::format("Could not read record {:d} from {:s}", i,
                                                      dbfFilePath.filename().string()));
            return false;
        }

        if (record.isDeleted()) {
            continue;
        }

        std::string categoryName;
        record.value(categoryName, "TEXT");
        categoryName = trimSpaces(categoryName);

        if (ownResourceCategoryName == categoryName) {
            readCustomCondition(record, customEventConditions().ownResource);
            customConditions = true;
        } else if (gameModeCategoryName == categoryName) {
            readCustomCondition(record, customEventConditions().gameMode);
            customConditions = true;
        }
    }

    return customConditions;
}

game::LEventCondCategoryTable* __fastcall eventCondCategoryTableCtorHooked(
    game::LEventCondCategoryTable* thisptr,
    int /*%edx*/,
    const char* globalsFolderPath,
    void* codeBaseEnvProxy)
{
    using namespace game;

    static const char dbfFileName[] = "LEvCond.dbf";
    const auto dbfFilePath{std::filesystem::path(globalsFolderPath) / dbfFileName};

    const bool customConditionsExist{readCustomConditions(dbfFilePath)};

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LEventCondCategoryTableApi::vftable();

    const auto& table = LEventCondCategoryTableApi::get();
    const auto& conditions = EventCondCategories::get();

    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);
    table.readCategory(conditions.frequency, thisptr, "L_FREQUENCY", dbfFileName);
    table.readCategory(conditions.location, thisptr, "L_LOCATION", dbfFileName);
    table.readCategory(conditions.enterCity, thisptr, "L_ENTER_CITY", dbfFileName);
    table.readCategory(conditions.ownCity, thisptr, "L_OWN_CITY", dbfFileName);
    table.readCategory(conditions.killStack, thisptr, "L_KILL_STACK", dbfFileName);
    table.readCategory(conditions.ownItem, thisptr, "L_OWN_ITEM", dbfFileName);
    table.readCategory(conditions.leaderOwnItem, thisptr, "L_LEADER_OWN_ITEM", dbfFileName);
    table.readCategory(conditions.diplomacy, thisptr, "L_DIPLOMACY", dbfFileName);
    table.readCategory(conditions.alliance, thisptr, "L_ALLIANCE", dbfFileName);
    table.readCategory(conditions.lootRuin, thisptr, "L_LOOT_RUIN", dbfFileName);
    table.readCategory(conditions.transformLand, thisptr, "L_TRANSFORM_LAND", dbfFileName);
    table.readCategory(conditions.visitSite, thisptr, "L_VISIT_SITE", dbfFileName);
    table.readCategory(conditions.leaderToZone, thisptr, "L_LEADER_TO_ZONE", dbfFileName);
    table.readCategory(conditions.leaderToCity, thisptr, "L_LEADER_TO_CITY", dbfFileName);
    table.readCategory(conditions.itemToLocation, thisptr, "L_ITEM_TO_LOCATION", dbfFileName);
    table.readCategory(conditions.stackExists, thisptr, "L_STACK_EXISTS", dbfFileName);
    table.readCategory(conditions.varInRange, thisptr, "L_VAR_IN_RANGE", dbfFileName);

    if (customConditionsExist) {
        table.readCategory(&customEventConditions().ownResource.category, thisptr,
                           ownResourceCategoryName, dbfFileName);
        table.readCategory(&customEventConditions().gameMode.category, thisptr,
                           gameModeCategoryName, dbfFileName);
    }

    table.initDone(thisptr);
    return thisptr;
}

} // namespace hooks
