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

#include "unitimplview.h"
#include "dynupgradeview.h"
#include "game.h"
#include "globaldata.h"
#include "log.h"
#include "ussoldier.h"
#include "usunitimpl.h"
#include "utils.h"
#include <fmt/format.h>
#include <sol/sol.hpp>

namespace bindings {

UnitImplView::UnitImplView(game::TUsUnitImpl* unitImpl)
    : impl(unitImpl)
{ }

void UnitImplView::bind(sol::state& lua)
{
    auto impl = lua.new_usertype<UnitImplView>("UnitImpl");
    impl["level"] = sol::property(&UnitImplView::getLevel);
    impl["xpNext"] = sol::property(&UnitImplView::getXpNext);
    impl["dynUpgLvl"] = sol::property(&UnitImplView::getDynUpgLevel);
    impl["dynUpg1"] = sol::property(&UnitImplView::getDynUpgrade1);
    impl["dynUpg2"] = sol::property(&UnitImplView::getDynUpgrade2);
}

int UnitImplView::getLevel() const
{
    using namespace game;

    auto soldier = gameFunctions().castUnitImplToSoldier(impl);
    if (!soldier) {
        hooks::logError("mssProxyError.log", fmt::format("Failed to cast unit impl {:s} to soldier",
                                                         hooks::idToString(&impl->unitId)));
        return 0;
    }

    auto vftable = static_cast<const IUsSoldierVftable*>(soldier->vftable);
    return vftable->getLevel(soldier);
}

int UnitImplView::getXpNext() const
{
    using namespace game;

    auto soldier = gameFunctions().castUnitImplToSoldier(impl);
    if (!soldier) {
        hooks::logError("mssProxyError.log", fmt::format("Failed to cast unit impl {:s} to soldier",
                                                         hooks::idToString(&impl->unitId)));
        return 0;
    }

    auto vftable = static_cast<const IUsSoldierVftable*>(soldier->vftable);
    return vftable->getXpNext(soldier);
}

int UnitImplView::getDynUpgLevel() const
{
    using namespace game;

    auto soldier = gameFunctions().castUnitImplToSoldier(impl);
    if (!soldier) {
        hooks::logError("mssProxyError.log", fmt::format("Failed to cast unit impl {:s} to soldier",
                                                         hooks::idToString(&impl->unitId)));
        return 0;
    }

    auto vftable = static_cast<const IUsSoldierVftable*>(soldier->vftable);
    return vftable->getDynUpgLvl(soldier);
}

std::optional<DynUpgradeView> UnitImplView::getDynUpgrade1() const
{
    using namespace game;

    auto soldier = gameFunctions().castUnitImplToSoldier(impl);
    auto vftable = static_cast<const IUsSoldierVftable*>(soldier->vftable);

    auto id = vftable->getDynUpg1(soldier);
    if (!id) {
        hooks::logError("mssProxyError.log", fmt::format("Dyn upgrade 1 id is null, unit impl {:s}",
                                                         hooks::idToString(&impl->unitId)));
        return std::nullopt;
    }

    const auto& globalApi = GlobalDataApi::get();
    auto globalData = *globalApi.getGlobalData();

    auto upgrade = globalApi.findDynUpgradeById(globalData->dynUpgrade, id);
    if (!upgrade) {
        hooks::logError("mssProxyError.log",
                        fmt::format("Could not find dyn upgrade 1 {:s}", hooks::idToString(id)));
        return std::nullopt;
    }

    return {upgrade};
}

std::optional<DynUpgradeView> UnitImplView::getDynUpgrade2() const
{
    using namespace game;

    auto soldier = gameFunctions().castUnitImplToSoldier(impl);
    auto vftable = static_cast<const IUsSoldierVftable*>(soldier->vftable);

    auto id = vftable->getDynUpg2(soldier);
    if (!id) {
        hooks::logError("mssProxyError.log", fmt::format("Dyn upgrade 2 id is null, unit impl {:s}",
                                                         hooks::idToString(&impl->unitId)));
        return std::nullopt;
    }

    const auto& globalApi = GlobalDataApi::get();
    auto globalData = *globalApi.getGlobalData();

    auto upgrade = globalApi.findDynUpgradeById(globalData->dynUpgrade, id);
    if (!upgrade) {
        hooks::logError("mssProxyError.log",
                        fmt::format("Could not find dyn upgrade 2 {:s}", hooks::idToString(id)));
        return std::nullopt;
    }

    return {upgrade};
}

} // namespace bindings
