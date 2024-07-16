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

#include "mercsview.h"
#include "midsitemercs.h"
#include "unitutils.h"
#include "usunitimpl.h"
#include <sol/sol.hpp>

namespace bindings {

MercenaryUnitView::MercenaryUnitView(const game::CMidgardID& unitImplId, bool unique)
    : unitImplId{unitImplId}
    , unique{unique}
{ }

void MercenaryUnitView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<MercenaryUnitView>("MercenaryUnitView");

    view["impl"] = sol::property(&MercenaryUnitView::getImpl);
    view["unique"] = sol::property(&MercenaryUnitView::isUnique);
}

UnitImplView MercenaryUnitView::getImpl() const
{
    return UnitImplView{hooks::getGlobalUnitImpl(&unitImplId)};
}

bool MercenaryUnitView::isUnique() const
{
    return unique;
}

MercsView::MercsView(const game::CMidSiteMercs* mercs, const game::IMidgardObjectMap* objectMap)
    : SiteView(mercs, objectMap)
{ }

void MercsView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<MercsView>("MercsView", sol::base_classes, sol::bases<SiteView>());

    bindAccessMethods(view);

    view["units"] = sol::property(&MercsView::getUnits);
}

std::vector<MercenaryUnitView> MercsView::getUnits() const
{
    const auto* mercs = static_cast<const game::CMidSiteMercs*>(site);

    std::vector<MercenaryUnitView> units;
    units.reserve(mercs->units.length);

    for (const auto& mercUnit : mercs->units) {
        units.emplace_back(mercUnit.unitId, mercUnit.unique);
    }

    return units;
}

} // namespace bindings
