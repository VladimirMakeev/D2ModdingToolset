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
#include "currencyview.h"
#include "globaldata.h"
#include "idview.h"
#include "lordtype.h"
#include "midplayer.h"
#include "racetype.h"
#include <sol/sol.hpp>

namespace bindings {

PlayerView::PlayerView(const game::CMidPlayer* player)
    : player{player}
{ }

void PlayerView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<PlayerView>("PlayerView");
    view["id"] = sol::property(&getId);
    view["race"] = sol::property(&getRaceCategoryId);
    view["lord"] = sol::property(&getLordCategoryId);
    view["bank"] = sol::property(&getBank);
    view["human"] = sol::property(&isHuman);
    view["alwaysAi"] = sol::property(&isAlwaysAi);
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

} // namespace bindings