/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "fortview.h"
#include "fortification.h"
#include "groupview.h"
#include "idview.h"
#include "itemview.h"
#include "midgardobjectmap.h"
#include "midsubrace.h"
#include <sol/sol.hpp>

namespace bindings {

FortView::FortView(const game::CFortification* fort, const game::IMidgardObjectMap* objectMap)
    : fort{fort}
    , objectMap{objectMap}
{ }

void FortView::bind(sol::state& lua)
{
    auto fortView = lua.new_usertype<FortView>("FortView");
    fortView["id"] = sol::property(&FortView::getId);
    fortView["group"] = sol::property(&FortView::getGroup);
    fortView["subrace"] = sol::property(&FortView::getSubrace);
    fortView["inventoryItems"] = sol::property(&FortView::getInventoryItems);
}

IdView FortView::getId() const
{
    return IdView{fort->id};
}

IdView FortView::getOwnerId() const
{
    return IdView{fort->ownerId};
}

IdView FortView::getStackId() const
{
    return IdView{fort->stackId};
}

GroupView FortView::getGroup() const
{
    return GroupView{&fort->group, objectMap, &fort->id};
}

int FortView::getSubrace() const
{
    auto obj{objectMap->vftable->findScenarioObjectById(objectMap, &fort->subraceId)};
    auto subrace{static_cast<const game::CMidSubRace*>(obj)};

    return subrace ? static_cast<int>(subrace->subraceCategory.id) : game::emptyCategoryId;
}

std::vector<ItemView> FortView::getInventoryItems() const
{
    const auto& items = fort->inventory.items;
    const auto count = items.end - items.bgn;

    std::vector<ItemView> result;
    result.reserve(count);

    for (const game::CMidgardID* it = items.bgn; it != items.end; it++) {
        result.push_back(ItemView{it, objectMap});
    }

    return result;
}

} // namespace bindings
