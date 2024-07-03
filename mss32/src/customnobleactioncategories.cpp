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

#include "customnobleactioncategories.h"
#include "dbffile.h"
#include "log.h"
#include "utils.h"
#include <array>
#include <filesystem>
#include <fmt/format.h>

namespace hooks {

static const char stealMarketActionCategoryName[]{"L_STEAL_MARKET"};

static CustomNobleActionCategories customCategories;

static void checkCustomActionCategories(const std::filesystem::path& dbfFilePath)
{
    utils::DbfFile dbf;
    if (!dbf.open(dbfFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbfFilePath.filename().string()));
        return;
    }

    const std::uint32_t recordsTotal{dbf.recordsTotal()};
    for (std::uint32_t i = 0u; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!dbf.record(record, i)) {
            continue;
        }

        if (record.isDeleted()) {
            continue;
        }

        std::string text;
        record.value(text, "TEXT");
        text = trimSpaces(text);

        if (text == stealMarketActionCategoryName) {
            auto& customCategories{getCustomNobleActionCategories()};
            customCategories.stealMarket.second = true;
            break;
        }
    }
}

CustomNobleActionCategories& getCustomNobleActionCategories()
{
    return customCategories;
}

game::LNobleActionCatTable* __fastcall nobleActionCatTableCtorHooked(
    game::LNobleActionCatTable* thisptr,
    int /*%edx*/,
    const char* globalsFolderPath,
    void* codeBaseEnvProxy)
{
    using namespace game;

    static const char dbfFileName[] = "LAction.dbf";
    checkCustomActionCategories(std::filesystem::path(globalsFolderPath) / dbfFileName);

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LNobleActionCatTableApi::vftable();

    const auto& table = LNobleActionCatTableApi::get();
    const auto& actions = NobleActionCategories::get();

    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);
    table.readCategory(actions.poisonStack, thisptr, "L_POISON_STACK", dbfFileName);
    table.readCategory(actions.spy, thisptr, "L_SPY", dbfFileName);
    table.readCategory(actions.stealItem, thisptr, "L_STEAL_ITEM", dbfFileName);
    table.readCategory(actions.assassinate, thisptr, "L_ASSASSINATE", dbfFileName);
    table.readCategory(actions.misfit, thisptr, "L_MISFIT", dbfFileName);
    table.readCategory(actions.duel, thisptr, "L_DUEL", dbfFileName);
    table.readCategory(actions.poisonCity, thisptr, "L_POISON_CITY", dbfFileName);
    table.readCategory(actions.stealSpell, thisptr, "L_STEAL_SPELL", dbfFileName);
    table.readCategory(actions.bribe, thisptr, "L_BRIBE", dbfFileName);
    table.readCategory(actions.stealGold, thisptr, "L_STEAL_GOLD", dbfFileName);
    table.readCategory(actions.stealMerchant, thisptr, "L_STEAL_MERCHANT", dbfFileName);
    table.readCategory(actions.stealMage, thisptr, "L_STEAL_MAGE", dbfFileName);
    table.readCategory(actions.spyRuin, thisptr, "L_SPY_RUIN", dbfFileName);
    table.readCategory(actions.riotCity, thisptr, "L_RIOT_CITY", dbfFileName);

    auto& customCategories{getCustomNobleActionCategories()};
    if (customCategories.stealMarket.second) {
        table.readCategory(&customCategories.stealMarket.first, thisptr,
                           stealMarketActionCategoryName, dbfFileName);
    }

    table.initDone(thisptr);
    return thisptr;
}

} // namespace hooks
