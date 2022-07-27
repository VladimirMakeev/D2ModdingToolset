/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#include "rodview.h"
#include "gameutils.h"
#include "midrod.h"
#include "playerview.h"
#include <sol/sol.hpp>

namespace bindings {

RodView::RodView(const game::CMidRod* rod, const game::IMidgardObjectMap* objectMap)
    : rod{rod}
    , objectMap{objectMap}
{ }

void RodView::bind(sol::state& lua)
{
    auto rodView = lua.new_usertype<RodView>("RodView");
    rodView["id"] = sol::property(&RodView::getId);
    rodView["position"] = sol::property(&RodView::getPosition);
    rodView["owner"] = sol::property(&RodView::getOwner);
}

IdView RodView::getId() const
{
    return IdView{rod->id};
}

Point RodView::getPosition() const
{
    return Point{rod->mapElement.position};
}

std::optional<PlayerView> RodView::getOwner() const
{
    auto player = hooks::getPlayer(objectMap, &rod->ownerId);
    if (!player) {
        return std::nullopt;
    }

    return {PlayerView{player}};
}

} // namespace bindings
