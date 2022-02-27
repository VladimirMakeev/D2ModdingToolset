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

#ifndef SCENARIOVIEW_H
#define SCENARIOVIEW_H

#include <optional>
#include <string>

namespace sol {
class state;
}

namespace game {
struct IMidgardObjectMap;
struct CScenarioInfo;
} // namespace game

namespace bindings {

struct IdView;
struct Point;
class LocationView;
class ScenVariablesView;
class TileView;

class ScenarioView
{
public:
    ScenarioView(const game::IMidgardObjectMap* objectMap);

    static void bind(sol::state& lua);

    /** Searches for location by id string. */
    std::optional<LocationView> getLocation(const std::string& id) const;
    /** Searches for location by id. */
    std::optional<LocationView> getLocationById(const IdView& id) const;
    /** Returns scenario variables, if present. */
    std::optional<ScenVariablesView> getScenVariables() const;

    /** Returns tile by coordinate pair. */
    std::optional<TileView> getTile(int x, int y) const;
    /** Returns tile by specified point. */
    std::optional<TileView> getTileByPoint(const Point& p) const;

    int getCurrentDay() const;
    int getSize() const;

private:
    const game::IMidgardObjectMap* objectMap;
};

} // namespace bindings

#endif // SCENARIOVIEW_H
