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
#include "unitutils.h"
#include "ussoldier.h"
#include "usunitimpl.h"
#include "utils.h"
#include <fmt/format.h>
#include <sol/sol.hpp>

namespace bindings {

UnitImplView::UnitImplView(game::IUsUnit* unitImpl)
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
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? soldier->vftable->getLevel(soldier) : 0;
}

int UnitImplView::getXpNext() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? soldier->vftable->getXpNext(soldier) : 0;
}

int UnitImplView::getDynUpgLevel() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? soldier->vftable->getDynUpgLvl(soldier) : 0;
}

std::optional<DynUpgradeView> UnitImplView::getDynUpgrade1() const
{
    return getDynUpgrade(1);
}

std::optional<DynUpgradeView> UnitImplView::getDynUpgrade2() const
{
    return getDynUpgrade(2);
}

std::optional<DynUpgradeView> UnitImplView::getDynUpgrade(int upgradeNumber) const
{
    using namespace game;

    if (upgradeNumber != 1 && upgradeNumber != 2)
        return std::nullopt;

    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    if (!soldier)
        return std::nullopt;

    auto id = upgradeNumber == 1 ? soldier->vftable->getDynUpg1(soldier)
                                 : soldier->vftable->getDynUpg2(soldier);
    if (!id) {
        hooks::logError("mssProxyError.log",
                        fmt::format("Dyn upgrade {:d} id is null, unit impl {:s}", upgradeNumber,
                                    hooks::idToString(&impl->unitId)));
        return std::nullopt;
    }

    const auto& globalApi = GlobalDataApi::get();
    auto globalData = *globalApi.getGlobalData();

    auto upgrade = globalApi.findDynUpgradeById(globalData->dynUpgrade, id);
    if (!upgrade) {
        hooks::logError("mssProxyError.log", fmt::format("Could not find dyn upgrade {:d} {:s}",
                                                         upgradeNumber, hooks::idToString(id)));
        return std::nullopt;
    }

    return {upgrade};
}

} // namespace bindings
