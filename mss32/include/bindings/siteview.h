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

#ifndef SITEVIEW_H
#define SITEVIEW_H

#include "idview.h"
#include "playerview.h"
#include "point.h"
#include <vector>

namespace sol {
class state;
}

namespace game {
struct CMidSite;
struct IMidgardObjectMap;
} // namespace game

namespace bindings {

class SiteView
{
public:
    SiteView(const game::CMidSite* site, const game::IMidgardObjectMap* objectMap);

    static void bind(sol::state& lua);

    IdView getId() const;
    Point getPosition() const;
    std::vector<PlayerView> getVisitors() const;

protected:
    template <typename T>
    static void bindAccessMethods(T& view)
    {
        view["id"] = sol::property(&SiteView::getId);
        view["position"] = sol::property(&SiteView::getPosition);
        view["visitors"] = sol::property(&SiteView::getVisitors);
    }

    const game::CMidSite* site;
    const game::IMidgardObjectMap* objectMap;
};

} // namespace bindings

#endif // SITEVIEW_H
