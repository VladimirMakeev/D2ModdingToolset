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

#include "locationview.h"
#include "idview.h"
#include "midlocation.h"
#include "point.h"
#include <sol/sol.hpp>

namespace bindings {

LocationView::LocationView(const game::CMidLocation* location)
    : location(location)
{ }

void LocationView::bind(sol::state& lua)
{
    auto location = lua.new_usertype<LocationView>("Location");
    location["id"] = sol::property(&LocationView::getId);
    location["position"] = sol::property(&LocationView::getPosition);
    location["radius"] = sol::property(&LocationView::getRadius);
}

IdView LocationView::getId() const
{
    return IdView{location->id};
}

Point LocationView::getPosition() const
{
    return Point{location->position};
}

int LocationView::getRadius() const
{
    // Convert radius from ingame type to actual value
    return location->radius * 2 + 1;
}

} // namespace bindings
