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

#include "siteview.h"
#include "gameutils.h"
#include "midsite.h"
#include <sol/sol.hpp>

namespace bindings {

SiteView::SiteView(const game::CMidSite* site, const game::IMidgardObjectMap* objectMap)
    : site{site}
    , objectMap{objectMap}
{ }

void SiteView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<SiteView>("SiteView");

    bindAccessMethods(view);
}

IdView SiteView::getId() const
{
    return IdView{site->id};
}

Point SiteView::getPosition() const
{
    return Point{site->mapElement.position};
}

std::vector<PlayerView> SiteView::getVisitors() const
{
    std::vector<PlayerView> visitors;
    visitors.reserve(site->visitors.length);

    for (const auto& playerId : site->visitors) {
        visitors.emplace_back(hooks::getPlayer(objectMap, &playerId), objectMap);
    }

    return visitors;
}

} // namespace bindings
