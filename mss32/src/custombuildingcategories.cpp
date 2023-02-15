/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#include "custombuildingcategories.h"
#include "dbfaccess.h"
#include "log.h"
#include <fmt/format.h>

namespace hooks {

static CustomBuildingCategories customBuildingCategories;

CustomBuildingCategories& getCustomBuildingCategories()
{
    return customBuildingCategories;
}

bool addCustomBuildingCategory(const std::filesystem::path& dbfFilePath,
                               game::BuildingBranchNumber branchNumber,
                               const char* text)
{
    using namespace game;

    if (!utils::dbValueExists(dbfFilePath, "TEXT", text)) {
        return false;
    }

    customBuildingCategories[branchNumber] = {nullptr, nullptr, (BuildingId)emptyCategoryId, text};
    logDebug("newBuildingType.log", fmt::format("Found custom building category {:s}", text));
    return true;
}

bool isCustomBuildingCategory(const game::LBuildingCategory* category)
{
    return std::find_if(customBuildingCategories.begin(), customBuildingCategories.end(),
                        [category](const CustomBuildingCategories::value_type& x) -> bool {
                            return x.second.id == category->id;
                        })
           != customBuildingCategories.end();
}

game::BuildingId getCustomBuildingCategoryId(game::BuildingBranchNumber branchNumber)
{
    using namespace game;

    auto category = customBuildingCategories.find(branchNumber);
    if (category != customBuildingCategories.end()) {
        return category->second.id;
    }

    return (BuildingId)emptyCategoryId;
}

} // namespace hooks
