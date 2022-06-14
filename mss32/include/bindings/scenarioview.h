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
struct CMidgardID;
struct IMidgardObjectMap;

enum class IdType : int;
} // namespace game

namespace bindings {

struct IdView;
struct Point;
class LocationView;
class ScenVariablesView;
class TileView;
class StackView;
class FortView;
class RuinView;
class UnitView;
class PlayerView;

/**
 * Returns stub values if objectMap is null.
 * This handles cases where some scripts can be called while the map is in a loading state.
 * See getScenarioMap utility for details.
 */
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

    /** Searches for stack by id string. */
    std::optional<StackView> getStack(const std::string& id) const;
    /** Searches for stack by id. */
    std::optional<StackView> getStackById(const IdView& id) const;
    /** Searches for stack by coordinate pair. */
    std::optional<StackView> getStackByCoordinates(int x, int y) const;
    /** Searches for stack at specified point. */
    std::optional<StackView> getStackByPoint(const Point& p) const;

    /** Searches for fort by id string. */
    std::optional<FortView> getFort(const std::string& id) const;
    /** Searches for fort by id. */
    std::optional<FortView> getFortById(const IdView& id) const;
    /** Searches for fort by coordinate pair. */
    std::optional<FortView> getFortByCoordinates(int x, int y) const;
    /** Searches for fort at specified point. */
    std::optional<FortView> getFortByPoint(const Point& p) const;

    /** Searches for ruin by id string. */
    std::optional<RuinView> getRuin(const std::string& id) const;
    /** Searches for ruin by id. */
    std::optional<RuinView> getRuinById(const IdView& id) const;
    /** Searches for ruin by coordinate pair. */
    std::optional<RuinView> getRuinByCoordinates(int x, int y) const;
    /** Searches for ruin at specified point. */
    std::optional<RuinView> getRuinByPoint(const Point& p) const;

    /** Searches for player by id string. */
    std::optional<PlayerView> getPlayer(const std::string& id) const;
    /** Searches for player by id. */
    std::optional<PlayerView> getPlayerById(const IdView& id) const;
    /** Searches for player that looted specified ruin. */
    std::optional<PlayerView> getPlayerByRuin(const RuinView& ruin) const;

    /** Searches for unit by id string. */
    std::optional<UnitView> getUnit(const std::string& id) const;
    /** Searches for unit by id. */
    std::optional<UnitView> getUnitById(const IdView& id) const;

    /** Searches for stack that has specified unit among all the stacks in the whole scenario. */
    std::optional<StackView> findStackByUnit(const UnitView& unit) const;
    std::optional<StackView> findStackByUnitId(const IdView& unitId) const;
    std::optional<StackView> findStackByUnitIdString(const std::string& unitId) const;

    /**
     * Searches for fort that has specified unit in its garrison among all the forts in the whole
     * scenario. Only garrison units are counted, visiting stack is ignored.
     */
    std::optional<FortView> findFortByUnit(const UnitView& unit) const;
    std::optional<FortView> findFortByUnitId(const IdView& unitId) const;
    std::optional<FortView> findFortByUnitIdString(const std::string& unitId) const;

    /** Searches for ruin that has specified unit among all the ruins in the whole scenario. */
    std::optional<RuinView> findRuinByUnit(const UnitView& unit) const;
    std::optional<RuinView> findRuinByUnitId(const IdView& unitId) const;
    std::optional<RuinView> findRuinByUnitIdString(const std::string& unitId) const;

    int getCurrentDay() const;
    int getSize() const;

private:
    const game::CMidgardID* getObjectId(int x, int y, game::IdType type) const;

    const game::IMidgardObjectMap* objectMap;
};

} // namespace bindings

#endif // SCENARIOVIEW_H
