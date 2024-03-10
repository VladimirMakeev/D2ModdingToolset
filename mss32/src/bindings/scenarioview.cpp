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

#include "scenarioview.h"
#include "crystalview.h"
#include "diplomacyview.h"
#include "dynamiccast.h"
#include "fortification.h"
#include "fortview.h"
#include "gameutils.h"
#include "idview.h"
#include "itemview.h"
#include "locationview.h"
#include "midcrystal.h"
#include "midgardmapblock.h"
#include "midgardobjectmap.h"
#include "midgardplan.h"
#include "midplayer.h"
#include "midrod.h"
#include "midruin.h"
#include "midscenvariables.h"
#include "midunit.h"
#include "playerview.h"
#include "point.h"
#include "rodview.h"
#include "ruinview.h"
#include "scenarioinfo.h"
#include "scenvariablesview.h"
#include "stackview.h"
#include "tileview.h"
#include "unitview.h"
#include <sol/sol.hpp>

namespace bindings {

ScenarioView::ScenarioView(const game::IMidgardObjectMap* objectMap)
    : objectMap(objectMap)
{ }

void ScenarioView::bind(sol::state& lua)
{
    auto scenario = lua.new_usertype<ScenarioView>("Scenario");
    scenario["getLocation"] = sol::overload<>(&ScenarioView::getLocation,
                                              &ScenarioView::getLocationById);
    scenario["variables"] = sol::property(&ScenarioView::getScenVariables);
    scenario["getTile"] = sol::overload<>(&ScenarioView::getTile, &ScenarioView::getTileByPoint);
    scenario["getStack"] = sol::overload<>(&ScenarioView::getStack, &ScenarioView::getStackById,
                                           &ScenarioView::getStackByCoordinates,
                                           &ScenarioView::getStackByPoint);
    scenario["getFort"] = sol::overload<>(&ScenarioView::getFort, &ScenarioView::getFortById,
                                          &ScenarioView::getFortByCoordinates,
                                          &ScenarioView::getFortByPoint);
    scenario["getRuin"] = sol::overload<>(&ScenarioView::getRuin, &ScenarioView::getRuinById,
                                          &ScenarioView::getRuinByCoordinates,
                                          &ScenarioView::getRuinByPoint);
    scenario["getRod"] = sol::overload<>(&ScenarioView::getRod, &ScenarioView::getRodById,
                                         &ScenarioView::getRodByCoordinates,
                                         &ScenarioView::getRodByPoint);
    scenario["getPlayer"] = sol::overload<>(&ScenarioView::getPlayer, &ScenarioView::getPlayerById);
    scenario["getUnit"] = sol::overload<>(&ScenarioView::getUnit, &ScenarioView::getUnitById);
    scenario["getItem"] = sol::overload<>(&ScenarioView::getItem, &ScenarioView::getItemById);
    scenario["getCrystal"] = sol::overload<>(&ScenarioView::getCrystal,
                                             &ScenarioView::getCrystalById,
                                             &ScenarioView::getCrystalByCoordinates,
                                             &ScenarioView::getCrystalByPoint);

    scenario["findStackByUnit"] = sol::overload<>(&ScenarioView::findStackByUnit,
                                                  &ScenarioView::findStackByUnitId,
                                                  &ScenarioView::findStackByUnitIdString);
    scenario["findFortByUnit"] = sol::overload<>(&ScenarioView::findFortByUnit,
                                                 &ScenarioView::findFortByUnitId,
                                                 &ScenarioView::findFortByUnitIdString);
    scenario["findRuinByUnit"] = sol::overload<>(&ScenarioView::findRuinByUnit,
                                                 &ScenarioView::findRuinByUnitId,
                                                 &ScenarioView::findRuinByUnitIdString);
    scenario["day"] = sol::property(&ScenarioView::getCurrentDay);
    scenario["size"] = sol::property(&ScenarioView::getSize);
    scenario["diplomacy"] = sol::property(&ScenarioView::getDiplomacy);
    scenario["forEachStack"] = &ScenarioView::forEachStack;
    scenario["forEachLocation"] = &ScenarioView::forEachLocation;
    scenario["forEachFort"] = &ScenarioView::forEachFort;
    scenario["forEachRuin"] = &ScenarioView::forEachRuin;
    scenario["forEachRod"] = &ScenarioView::forEachRod;
    scenario["forEachPlayer"] = &ScenarioView::forEachPlayer;
    scenario["forEachUnit"] = &ScenarioView::forEachUnit;
    scenario["forEachCrystal"] = &ScenarioView::forEachCrystal;
}

std::optional<LocationView> ScenarioView::getLocation(const std::string& id) const
{
    return getLocationById(IdView{id});
}

std::optional<LocationView> ScenarioView::getLocationById(const IdView& id) const
{
    using namespace game;

    if (!objectMap) {
        return std::nullopt;
    }

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, &id.id);
    if (!obj) {
        return std::nullopt;
    }

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();

    auto* location = (const game::CMidLocation*)dynamicCast(obj, 0, rtti.IMidScenarioObjectType,
                                                            rtti.CMidLocationType, 0);
    if (!location) {
        return std::nullopt;
    }

    return LocationView{location};
}

std::optional<ScenVariablesView> ScenarioView::getScenVariables() const
{
    if (!objectMap) {
        return std::nullopt;
    }

    auto variables{hooks::getScenarioVariables(objectMap)};
    if (!variables) {
        return std::nullopt;
    }

    return ScenVariablesView{variables};
}

std::optional<TileView> ScenarioView::getTile(int x, int y) const
{
    using namespace game;

    if (!objectMap) {
        return std::nullopt;
    }

    auto info = hooks::getScenarioInfo(objectMap);
    if (!info) {
        return std::nullopt;
    }

    auto block = hooks::getMidgardMapBlock(objectMap, &info->id, info->mapSize, x, y);
    if (!block) {
        return std::nullopt;
    }

    const auto& blockPos = block->position;
    if (x < blockPos.x || x >= blockPos.x + 8 || y < blockPos.y || y >= blockPos.y + 4) {
        // Outside of map block
        return std::nullopt;
    }

    const auto index = x + 8 * (y - blockPos.y) - blockPos.x;
    if (index < 0 || index >= 32) {
        return std::nullopt;
    }

    return TileView{block->tiles[index]};
}

std::optional<TileView> ScenarioView::getTileByPoint(const Point& p) const
{
    return getTile(p.x, p.y);
}

std::optional<StackView> ScenarioView::getStack(const std::string& id) const
{
    return getStackById(IdView{id});
}

std::optional<StackView> ScenarioView::getStackById(const IdView& id) const
{
    if (!objectMap) {
        return std::nullopt;
    }

    auto stack = hooks::getStack(objectMap, &id.id);
    if (!stack) {
        return std::nullopt;
    }

    return StackView{stack, objectMap};
}

std::optional<StackView> ScenarioView::getStackByCoordinates(int x, int y) const
{
    auto stackId = getObjectId(x, y, game::IdType::Stack);
    if (!stackId) {
        return std::nullopt;
    }

    return getStackById(IdView{stackId});
}

std::optional<StackView> ScenarioView::getStackByPoint(const Point& p) const
{
    return getStackByCoordinates(p.x, p.y);
}

std::optional<StackView> ScenarioView::findStackByUnit(const UnitView& unit) const
{
    auto unitId = unit.getId();
    return findStackByUnitId(unitId);
}

std::optional<StackView> ScenarioView::findStackByUnitId(const IdView& unitId) const
{
    if (!objectMap) {
        return std::nullopt;
    }

    auto stack = hooks::getStackByUnitId(objectMap, &unitId.id);
    if (!stack) {
        return std::nullopt;
    }

    return {StackView{stack, objectMap}};
}

std::optional<StackView> ScenarioView::findStackByUnitIdString(const std::string& unitId) const
{
    return findStackByUnitId(IdView{unitId});
}

std::optional<FortView> ScenarioView::getFort(const std::string& id) const
{
    return getFortById(IdView{id});
}

std::optional<FortView> ScenarioView::getFortById(const IdView& id) const
{
    using namespace game;

    if (!objectMap) {
        return std::nullopt;
    }

    if (CMidgardIDApi::get().getType(&id.id) != IdType::Fortification) {
        return std::nullopt;
    }

    auto fort = hooks::getFort(objectMap, &id.id);
    if (!fort) {
        return std::nullopt;
    }

    return {FortView{fort, objectMap}};
}

std::optional<FortView> ScenarioView::getFortByCoordinates(int x, int y) const
{
    auto fortId = getObjectId(x, y, game::IdType::Fortification);
    if (!fortId) {
        return std::nullopt;
    }

    return getFortById(IdView{fortId});
}

std::optional<FortView> ScenarioView::getFortByPoint(const Point& p) const
{
    return getFortByCoordinates(p.x, p.y);
}

std::optional<FortView> ScenarioView::findFortByUnit(const UnitView& unit) const
{
    auto unitId = unit.getId();
    return findFortByUnitId(unitId);
}

std::optional<FortView> ScenarioView::findFortByUnitId(const IdView& unitId) const
{
    if (!objectMap) {
        return std::nullopt;
    }

    auto fort = hooks::getFortByUnitId(objectMap, &unitId.id);
    if (!fort) {
        return std::nullopt;
    }

    return {FortView{fort, objectMap}};
}

std::optional<FortView> ScenarioView::findFortByUnitIdString(const std::string& unitId) const
{
    return findFortByUnitId(IdView{unitId});
}

std::optional<RuinView> ScenarioView::getRuin(const std::string& id) const
{
    return getRuinById(IdView{id});
}

std::optional<RuinView> ScenarioView::getRuinById(const IdView& id) const
{
    using namespace game;

    if (!objectMap) {
        return std::nullopt;
    }

    if (CMidgardIDApi::get().getType(&id.id) != IdType::Ruin) {
        return std::nullopt;
    }

    auto ruin = hooks::getRuin(objectMap, &id.id);
    if (!ruin) {
        return std::nullopt;
    }

    return {RuinView{ruin, objectMap}};
}

std::optional<RuinView> ScenarioView::getRuinByCoordinates(int x, int y) const
{
    auto ruinId = getObjectId(x, y, game::IdType::Ruin);
    if (!ruinId) {
        return std::nullopt;
    }

    return getRuinById(IdView{ruinId});
}

std::optional<RuinView> ScenarioView::getRuinByPoint(const Point& p) const
{
    return getRuinByCoordinates(p.x, p.y);
}

std::optional<RodView> ScenarioView::getRod(const std::string& id) const
{
    return getRodById(IdView{id});
}

std::optional<RodView> ScenarioView::getRodById(const IdView& id) const
{
    using namespace game;

    if (!objectMap) {
        return std::nullopt;
    }

    if (CMidgardIDApi::get().getType(&id.id) != IdType::Rod) {
        return std::nullopt;
    }

    auto rod = hooks::getRod(objectMap, &id.id);
    if (!rod) {
        return std::nullopt;
    }

    return {RodView{rod, objectMap}};
}

std::optional<RodView> ScenarioView::getRodByCoordinates(int x, int y) const
{
    auto rodId = getObjectId(x, y, game::IdType::Rod);
    if (!rodId) {
        return std::nullopt;
    }

    return getRodById(IdView{rodId});
}

std::optional<RodView> ScenarioView::getRodByPoint(const Point& p) const
{
    return getRodByCoordinates(p.x, p.y);
}

std::optional<RuinView> ScenarioView::findRuinByUnit(const UnitView& unit) const
{
    auto unitId = unit.getId();
    return findRuinByUnitId(unitId);
}

std::optional<RuinView> ScenarioView::findRuinByUnitId(const IdView& unitId) const
{
    if (!objectMap) {
        return std::nullopt;
    }

    auto ruin = hooks::getRuinByUnitId(objectMap, &unitId.id);
    if (!ruin) {
        return std::nullopt;
    }

    return {RuinView{ruin, objectMap}};
}

std::optional<RuinView> ScenarioView::findRuinByUnitIdString(const std::string& unitId) const
{
    return findRuinByUnitId(IdView{unitId});
}

std::optional<PlayerView> ScenarioView::getPlayer(const std::string& id) const
{
    return getPlayerById(IdView{id});
}

std::optional<PlayerView> ScenarioView::getPlayerById(const IdView& id) const
{
    using namespace game;

    if (!objectMap) {
        return std::nullopt;
    }

    if (CMidgardIDApi::get().getType(&id.id) != IdType::Player) {
        return std::nullopt;
    }

    auto player = hooks::getPlayer(objectMap, &id.id);
    if (!player) {
        return std::nullopt;
    }

    return {PlayerView{player, objectMap}};
}

std::optional<UnitView> ScenarioView::getUnit(const std::string& id) const
{
    return getUnitById(IdView{id});
}

std::optional<UnitView> ScenarioView::getUnitById(const IdView& id) const
{
    using namespace game;

    if (!objectMap) {
        return std::nullopt;
    }

    if (CMidgardIDApi::get().getType(&id.id) != IdType::Unit) {
        return std::nullopt;
    }

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, &id.id);
    if (!obj) {
        return std::nullopt;
    }

    return {UnitView{(const CMidUnit*)obj}};
}

std::optional<ItemView> ScenarioView::getItem(const std::string& id) const
{
    return getItemById(IdView{id});
}

std::optional<ItemView> ScenarioView::getItemById(const IdView& id) const
{
    using namespace game;

    if (!objectMap) {
        return std::nullopt;
    }

    if (CMidgardIDApi::get().getType(&id.id) != IdType::Item) {
        return std::nullopt;
    }

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, &id.id);
    if (!obj) {
        return std::nullopt;
    }

    return {ItemView{&id.id, objectMap}};
}

std::optional<CrystalView> ScenarioView::getCrystal(const std::string& id) const
{
    return getCrystalById(IdView{id});
}

std::optional<CrystalView> ScenarioView::getCrystalById(const IdView& id) const
{
    using namespace game;

    if (!objectMap) {
        return std::nullopt;
    }

    if (CMidgardIDApi::get().getType(&id.id) != IdType::Crystal) {
        return std::nullopt;
    }

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, &id.id);
    if (!obj) {
        return std::nullopt;
    }

    return {CrystalView{static_cast<const CMidCrystal*>(obj)}};
}

std::optional<CrystalView> ScenarioView::getCrystalByCoordinates(int x, int y) const
{
    auto crystalId = getObjectId(x, y, game::IdType::Crystal);
    if (!crystalId) {
        return std::nullopt;
    }

    return getCrystalById(IdView{crystalId});
}

std::optional<CrystalView> ScenarioView::getCrystalByPoint(const Point& p) const
{
    return getCrystalByCoordinates(p.x, p.y);
}

int ScenarioView::getCurrentDay() const
{
    if (!objectMap) {
        return 0;
    }

    auto info = hooks::getScenarioInfo(objectMap);
    return info ? info->currentTurn : 0;
}

int ScenarioView::getSize() const
{
    if (!objectMap) {
        return 0;
    }

    auto info = hooks::getScenarioInfo(objectMap);
    return info ? info->mapSize : 0;
}

std::optional<DiplomacyView> ScenarioView::getDiplomacy() const
{
    if (!objectMap) {
        return std::nullopt;
    }

    return DiplomacyView{hooks::getDiplomacy(objectMap)};
}

void ScenarioView::forEachStack(const std::function<void(const StackView&)>& callback) const
{
    if (!objectMap) {
        return;
    }

    using namespace game;

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();

    auto runCallback = [this, &callback, &dynamicCast, &rtti](const IMidScenarioObject* obj) {
        auto* stack = (const CMidStack*)dynamicCast(obj, 0, rtti.IMidScenarioObjectType,
                                                    rtti.CMidStackType, 0);

        const StackView stackView{stack, objectMap};
        callback(stackView);
    };

    hooks::forEachScenarioObject(objectMap, IdType::Stack, runCallback);
}

void ScenarioView::forEachLocation(const std::function<void(const LocationView&)>& callback) const
{
    if (!objectMap) {
        return;
    }

    using namespace game;

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();

    auto runCallback = [&callback, &dynamicCast, &rtti](const IMidScenarioObject* obj) {
        auto* location = (const CMidLocation*)dynamicCast(obj, 0, rtti.IMidScenarioObjectType,
                                                          rtti.CMidLocationType, 0);

        const LocationView locationView{location};
        callback(locationView);
    };

    hooks::forEachScenarioObject(objectMap, IdType::Location, runCallback);
}

void ScenarioView::forEachFort(const std::function<void(const FortView&)>& callback) const
{
    if (!objectMap) {
        return;
    }

    using namespace game;

    auto runCallback = [this, &callback](const IMidScenarioObject* obj) {
        auto* fort{static_cast<const CFortification*>(obj)};

        const FortView fortView{fort, objectMap};
        callback(fortView);
    };

    hooks::forEachScenarioObject(objectMap, IdType::Fortification, runCallback);
}

void ScenarioView::forEachRuin(const std::function<void(const RuinView&)>& callback) const
{
    if (!objectMap) {
        return;
    }

    using namespace game;

    auto runCallback = [this, &callback](const IMidScenarioObject* obj) {
        auto* ruin{static_cast<const CMidRuin*>(obj)};

        const RuinView ruinView{ruin, objectMap};
        callback(ruinView);
    };

    hooks::forEachScenarioObject(objectMap, IdType::Ruin, runCallback);
}

void ScenarioView::forEachRod(const std::function<void(const RodView&)>& callback) const
{
    if (!objectMap) {
        return;
    }

    using namespace game;

    auto runCallback = [this, &callback](const IMidScenarioObject* obj) {
        auto* rod{static_cast<const CMidRod*>(obj)};

        const RodView rodView{rod, objectMap};
        callback(rodView);
    };

    hooks::forEachScenarioObject(objectMap, IdType::Rod, runCallback);
}

void ScenarioView::forEachPlayer(const std::function<void(const PlayerView&)>& callback) const
{
    if (!objectMap) {
        return;
    }

    using namespace game;

    auto runCallback = [this, &callback](const IMidScenarioObject* obj) {
        auto* player{static_cast<const CMidPlayer*>(obj)};

        const PlayerView playerView{player, objectMap};
        callback(playerView);
    };

    hooks::forEachScenarioObject(objectMap, IdType::Player, runCallback);
}

void ScenarioView::forEachUnit(const std::function<void(const UnitView&)>& callback) const
{
    if (!objectMap) {
        return;
    }

    using namespace game;

    auto runCallback = [&callback](const IMidScenarioObject* obj) {
        auto* unit{static_cast<const CMidUnit*>(obj)};

        const UnitView unitView{unit};
        callback(unitView);
    };

    hooks::forEachScenarioObject(objectMap, IdType::Unit, runCallback);
}

void ScenarioView::forEachCrystal(const std::function<void(const CrystalView&)>& callback) const
{
    if (!objectMap) {
        return;
    }

    using namespace game;

    auto runCallback = [&callback](const IMidScenarioObject* obj) {
        auto* crystal{static_cast<const CMidCrystal*>(obj)};

        const CrystalView crystalView{crystal};
        callback(crystalView);
    };

    hooks::forEachScenarioObject(objectMap, IdType::Crystal, runCallback);
}

const game::CMidgardID* ScenarioView::getObjectId(int x, int y, game::IdType type) const
{
    using namespace game;

    if (!objectMap) {
        return nullptr;
    }

    auto plan{hooks::getMidgardPlan(objectMap)};
    if (!plan) {
        return nullptr;
    }

    const CMqPoint position{x, y};
    return CMidgardPlanApi::get().getObjectId(plan, &position, &type);
}

} // namespace bindings
