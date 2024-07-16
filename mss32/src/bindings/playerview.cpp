/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "playerview.h"
#include "fogview.h"
#include "gameutils.h"
#include "globaldata.h"
#include "lordtype.h"
#include "midplayer.h"
#include "playerbuildings.h"
#include "racetype.h"
#include <sol/sol.hpp>

namespace bindings {

PlayerView::PlayerView(const game::CMidPlayer* player, const game::IMidgardObjectMap* objectMap)
    : player{player}
    , objectMap{objectMap}
{ }

void PlayerView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<PlayerView>("PlayerView");
    view["id"] = sol::property(&PlayerView::getId);
    view["race"] = sol::property(&PlayerView::getRaceCategoryId);
    view["lord"] = sol::property(&PlayerView::getLordCategoryId);
    view["bank"] = sol::property(&PlayerView::getBank);
    view["human"] = sol::property(&PlayerView::isHuman);
    view["alwaysAi"] = sol::property(&PlayerView::isAlwaysAi);
    view["fog"] = sol::property(&PlayerView::getFog);
    view["buildings"] = sol::property(&PlayerView::getBuildings);
    view["hasBuilding"] = sol::overload<>(&PlayerView::hasBuilding, &PlayerView::hasBuildingById);
}

IdView PlayerView::getId() const
{
    return player->id;
}

int PlayerView::getRaceCategoryId() const
{
    const auto raceType = player->raceType;
    if (!raceType)
        return game::emptyCategoryId;

    return (int)raceType->data->raceType.id;
}

int PlayerView::getLordCategoryId() const
{
    using namespace game;

    const auto& globalApi = GlobalDataApi::get();

    const auto lords = (*globalApi.getGlobalData())->lords;
    const auto lordType = (TLordType*)globalApi.findById(lords, &player->lordId);
    if (!lordType)
        return game::emptyCategoryId;

    return (int)lordType->data->lordCategory.id;
}

CurrencyView PlayerView::getBank() const
{
    return {player->bank};
}

bool PlayerView::isHuman() const
{
    return player->isHuman;
}

bool PlayerView::isAlwaysAi() const
{
    return player->alwaysAi;
}

std::optional<FogView> PlayerView::getFog() const
{
    const auto fog{hooks::getFog(objectMap, player)};
    if (!fog) {
        return std::nullopt;
    }

    return FogView{fog};
}

std::vector<BuildingView> PlayerView::getBuildings() const
{
    using namespace game;

    std::vector<BuildingView> buildings;

    auto playerBuildings{hooks::getPlayerBuildings(objectMap, player)};
    if (!playerBuildings) {
        return buildings;
    }

    const auto& buildingsList{playerBuildings->buildings};

    const auto& globalApi{GlobalDataApi::get()};
    const GlobalData* global{*globalApi.getGlobalData()};

    buildings.reserve(buildingsList.length);
    for (auto node = buildingsList.head->next; node != buildingsList.head; node = node->next) {
        auto buildingType{(const TBuildingType*)globalApi.findById(global->buildings, &node->data)};
        if (!buildingType) {
            continue;
        }

        buildings.push_back(BuildingView{buildingType});
    }

    return buildings;
}

bool PlayerView::hasBuilding(const std::string& id) const
{
    return hasBuildingById(IdView{id});
}

bool PlayerView::hasBuildingById(const IdView& id) const
{
    auto playerBuildings{hooks::getPlayerBuildings(objectMap, player)};
    if (!playerBuildings) {
        return false;
    }

    const auto& buildings{playerBuildings->buildings};

    for (auto node = buildings.head->next; node != buildings.head; node = node->next) {
        if (node->data == id.id) {
            return true;
        }
    }

    return false;
}

} // namespace bindings
