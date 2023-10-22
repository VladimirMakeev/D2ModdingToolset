/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#include "groupview.h"
#include "game.h"
#include "idview.h"
#include "midunitgroup.h"
#include "unitslotview.h"
#include "unitview.h"
#include <sol/sol.hpp>

namespace bindings {

GroupView::GroupView(const game::CMidUnitGroup* group,
                     const game::IMidgardObjectMap* objectMap,
                     const game::CMidgardID* groupId)
    : group{group}
    , objectMap{objectMap}
    , groupId{*groupId}
{ }

void GroupView::bind(sol::state& lua)
{
    auto group = lua.new_usertype<GroupView>("GroupView");
    group["id"] = sol::property(&GroupView::getId);
    group["slots"] = sol::property(&GroupView::getSlots);
    group["units"] = sol::property(&GroupView::getUnits);
    group["hasUnit"] = sol::overload<>(&GroupView::hasUnit, &GroupView::hasUnitById);
}

IdView GroupView::getId() const
{
    return IdView{groupId};
}

GroupView::GroupSlots GroupView::getSlots() const
{
    const auto& fn = game::gameFunctions();
    GroupSlots slots;

    for (size_t i = 0; i < std::size(group->positions); ++i) {
        const auto& unitId{group->positions[i]};

        if (unitId == game::emptyId) {
            slots.emplace_back(UnitSlotView(nullptr, i, &groupId));
            continue;
        }

        // Unit can be null in case where a map is in a state of loading, group object is already
        // serialized, while some (or all) of its units are not (yet).
        auto unit = fn.findUnitById(objectMap, &unitId);
        if (unit) {
            slots.emplace_back(UnitSlotView(unit, i, &groupId));
        }
    }

    return slots;
}

GroupView::GroupUnits GroupView::getUnits() const
{
    const auto& fn = game::gameFunctions();
    GroupUnits units;

    for (const game::CMidgardID* it = group->units.bgn; it != group->units.end; ++it) {
        auto unit = fn.findUnitById(objectMap, it);
        if (unit) {
            units.emplace_back(UnitView(unit));
        }
    }

    return units;
}

bool GroupView::hasUnit(const bindings::UnitView& unit) const
{
    auto unitId = unit.getId();
    return hasUnitById(unitId);
}

bool GroupView::hasUnitById(const bindings::IdView& unitId) const
{
    auto& units = group->units;
    for (const game::CMidgardID* it = units.bgn; it != units.end; ++it) {
        if (unitId.id == *it)
            return true;
    }

    return false;
}

} // namespace bindings
