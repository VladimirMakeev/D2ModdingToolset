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

#include "sitecategoryhooks.h"
#include "dbf/dbffile.h"
#include "log.h"
#include "midsiteresourcemarket.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

static const char* resourceMarketSiteCategoryName{"L_RESOURCE_MARKET"};

CustomSiteCategories& customSiteCategories()
{
    static CustomSiteCategories customSites{};

    return customSites;
}

static bool readCustomSites(const std::filesystem::path& dbfFilePath)
{
    utils::DbfFile dbf;
    if (!dbf.open(dbfFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbfFilePath.filename().string()));
        return false;
    }

    const std::uint32_t recordsTotal{dbf.recordsTotal()};
    for (std::uint32_t i = 0u; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!dbf.record(record, i)) {
            logError("mssProxyError.log", fmt::format("Could not read record {:d} from {:s}", i,
                                                      dbfFilePath.filename().string()));
            return false;
        }

        if (record.isDeleted()) {
            continue;
        }

        std::string siteName;
        record.value(siteName, "TEXT");
        siteName = trimSpaces(siteName);

        if (siteName == resourceMarketSiteCategoryName) {
            std::string scriptPath;
            record.value(scriptPath, "SCRIPT");
            customSiteCategories().exchangeRatesScript = trimSpaces(scriptPath);
            return true;
        }
    }

    return false;
}

game::LSiteCategoryTable* __fastcall siteCategoryTableCtorHooked(game::LSiteCategoryTable* thisptr,
                                                                 int /*%edx*/,
                                                                 const char* globalsFolderPath,
                                                                 void* codeBaseEnvProxy)
{
    using namespace game;

    static const char dbfFileName[] = "LSite.dbf";
    const auto dbfFilePath{std::filesystem::path(globalsFolderPath) / dbfFileName};

    const bool customSitesExist{readCustomSites(dbfFilePath)};
    customSiteCategories().exists = customSitesExist;

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LSiteCategoryTableApi::vftable();

    const auto& table = LSiteCategoryTableApi::get();
    const auto& sites = SiteCategories::get();

    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);
    table.readCategory(sites.merchant, thisptr, "L_MERCHANT", dbfFileName);
    table.readCategory(sites.mageTower, thisptr, "L_MAGE_TOWER", dbfFileName);
    table.readCategory(sites.mercenaries, thisptr, "L_MERCENARIES", dbfFileName);
    table.readCategory(sites.trainer, thisptr, "L_TRAINER", dbfFileName);

    if (customSitesExist) {
        table.readCategory(&customSiteCategories().resourceMarket, thisptr,
                           resourceMarketSiteCategoryName, dbfFileName);
        // Register CStreamRegister for our new site
        addResourceMarketStreamRegister();
    }

    table.initDone(thisptr);
    return thisptr;
}

} // namespace hooks
