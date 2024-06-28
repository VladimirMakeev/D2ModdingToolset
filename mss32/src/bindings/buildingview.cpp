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

#include "buildingview.h"
#include "buildingtype.h"
#include "globaldata.h"
#include <sol/sol.hpp>

namespace bindings {

BuildingView::BuildingView(const game::TBuildingType* building)
    : building{building}
{ }

void BuildingView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<BuildingView>("BuildingView");
    view["id"] = sol::property(&BuildingView::getId);
    view["cost"] = sol::property(&BuildingView::getCost);
    view["type"] = sol::property(&BuildingView::getCategory);
    view["requiredBuilding"] = sol::property(&BuildingView::getRequiredBuilding);
    view["branch"] = sol::property(&BuildingView::getUnitBranch);
    view["level"] = sol::property(&BuildingView::getLevel);
}

IdView BuildingView::getId() const
{
    return IdView{building->id};
}

CurrencyView BuildingView::getCost() const
{
    return CurrencyView{building->data->cost};
}

int BuildingView::getCategory() const
{
    return static_cast<int>(building->data->category.id);
}

std::optional<BuildingView> BuildingView::getRequiredBuilding() const
{
    using namespace game;

    const CMidgardID& requiredId{building->data->requiredId};
    if (requiredId == emptyId || requiredId == invalidId) {
        return std::nullopt;
    }

    const auto& globalApi{GlobalDataApi::get()};
    const GlobalData* global{*globalApi.getGlobalData()};

    auto requiredBuilding{(const TBuildingType*)globalApi.findById(global->buildings, &requiredId)};
    if (!requiredBuilding) {
        return std::nullopt;
    }

    return BuildingView{requiredBuilding};
}

int BuildingView::getUnitBranch() const
{
    using namespace game;

    if (building->data->category.id != BuildingCategories::get().unit->id) {
        return emptyCategoryId;
    }

    auto unitBuilding{reinterpret_cast<const TBuildingUnitUpgType*>(building)};
    return static_cast<int>(unitBuilding->branch.id);
}

int BuildingView::getLevel() const
{
    using namespace game;

    if (building->data->category.id != BuildingCategories::get().unit->id) {
        return 0;
    }

    auto unitBuilding{reinterpret_cast<const TBuildingUnitUpgType*>(building)};
    return unitBuilding->level;
}

} // namespace bindings
