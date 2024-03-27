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

#include <functional>
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
class RodView;
class DiplomacyView;
class ItemView;
class CrystalView;
class MerchantView;
class MercsView;
class TrainerView;

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

    /** Searches for rod by id string. */
    std::optional<RodView> getRod(const std::string& id) const;
    /** Searches for rod by id. */
    std::optional<RodView> getRodById(const IdView& id) const;
    /** Searches for rod by coordinate pair. */
    std::optional<RodView> getRodByCoordinates(int x, int y) const;
    /** Searches for rod at specified point. */
    std::optional<RodView> getRodByPoint(const Point& p) const;

    /** Searches for player by id string. */
    std::optional<PlayerView> getPlayer(const std::string& id) const;
    /** Searches for player by id. */
    std::optional<PlayerView> getPlayerById(const IdView& id) const;

    /** Searches for unit by id string. */
    std::optional<UnitView> getUnit(const std::string& id) const;
    /** Searches for unit by id. */
    std::optional<UnitView> getUnitById(const IdView& id) const;

    /** Searches for item by id string. */
    std::optional<ItemView> getItem(const std::string& id) const;
    /** Searches for item by id. */
    std::optional<ItemView> getItemById(const IdView& id) const;

    /** Searches for crystal by id string. */
    std::optional<CrystalView> getCrystal(const std::string& id) const;
    /** Searches for crystal by id. */
    std::optional<CrystalView> getCrystalById(const IdView& id) const;
    /** Searches for crystal by coordinate pair. */
    std::optional<CrystalView> getCrystalByCoordinates(int x, int y) const;
    /** Searches for crystal at specified point. */
    std::optional<CrystalView> getCrystalByPoint(const Point& p) const;

    /** Searches for merchant by id string. */
    std::optional<MerchantView> getMerchant(const std::string& id) const;
    /** Searches for merchant by id. */
    std::optional<MerchantView> getMerchantById(const IdView& id) const;
    /** Searches for merchant by coordinate pair. */
    std::optional<MerchantView> getMerchantByCoordinates(int x, int y) const;
    /** Searches for merchant at specified point. */
    std::optional<MerchantView> getMerchantByPoint(const Point& p) const;

    /** Searches for mercenaries by id string. */
    std::optional<MercsView> getMercs(const std::string& id) const;
    /** Searches for mercenaries by id. */
    std::optional<MercsView> getMercsById(const IdView& id) const;
    /** Searches for mercenaries by coordinate pair. */
    std::optional<MercsView> getMercsByCoordinates(int x, int y) const;
    /** Searches for mercenaries at specified point. */
    std::optional<MercsView> getMercsByPoint(const Point& p) const;

    /** Searches for trainer by id string. */
    std::optional<TrainerView> getTrainer(const std::string& id) const;
    /** Searches for trainer by id. */
    std::optional<TrainerView> getTrainerById(const IdView& id) const;
    /** Searches for trainer by coordinate pair. */
    std::optional<TrainerView> getTrainerByCoordinates(int x, int y) const;
    /** Searches for trainer at specified point. */
    std::optional<TrainerView> getTrainerByPoint(const Point& p) const;

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

    std::string getName() const;
    std::string getDescription() const;
    std::string getAuthor() const;
    std::uint32_t getSeed() const;
    int getCurrentDay() const;
    int getSize() const;

    std::optional<DiplomacyView> getDiplomacy() const;

    void forEachStack(const std::function<void(const StackView&)>& callback) const;
    void forEachLocation(const std::function<void(const LocationView&)>& callback) const;
    void forEachFort(const std::function<void(const FortView&)>& callback) const;
    void forEachRuin(const std::function<void(const RuinView&)>& callback) const;
    void forEachRod(const std::function<void(const RodView&)>& callback) const;
    void forEachPlayer(const std::function<void(const PlayerView&)>& callback) const;
    void forEachUnit(const std::function<void(const UnitView&)>& callback) const;
    void forEachCrystal(const std::function<void(const CrystalView&)>& callback) const;
    void forEachMerchant(const std::function<void(const MerchantView&)>& callback) const;
    void forEachMercenary(const std::function<void(const MercsView&)>& callback) const;
    void forEachTrainer(const std::function<void(const TrainerView&)>& callback) const;

private:
    const game::CMidgardID* getObjectId(int x, int y, game::IdType type) const;

    const game::IMidgardObjectMap* objectMap;
};

} // namespace bindings

#endif // SCENARIOVIEW_H
