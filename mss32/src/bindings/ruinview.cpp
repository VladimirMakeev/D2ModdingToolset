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

#include "ruinview.h"
#include "currencyview.h"
#include "groupview.h"
#include "idview.h"
#include "itemview.h"
#include "midruin.h"
#include <sol/sol.hpp>

namespace bindings {

RuinView::RuinView(const game::CMidRuin* ruin, const game::IMidgardObjectMap* objectMap)
    : ruin{ruin}
    , objectMap{objectMap}
{ }

void RuinView::bind(sol::state& lua)
{
    auto ruinView = lua.new_usertype<RuinView>("RuinView");
    ruinView["id"] = sol::property(&RuinView::getId);
    ruinView["group"] = sol::property(&RuinView::getGroup);
    ruinView["item"] = sol::property(&RuinView::getItem);
    ruinView["cash"] = sol::property(&RuinView::getCash);
}

IdView RuinView::getId() const
{
    return IdView{ruin->id};
}

IdView RuinView::getLooterId() const
{
    return IdView{ruin->looterId};
}

GroupView RuinView::getGroup() const
{
    return GroupView{&ruin->group, objectMap, &ruin->id};
}

std::optional<ItemView> RuinView::getItem() const
{
    if (ruin->itemId == game::emptyId)
        return std::nullopt;

    return {ItemView{&ruin->itemId, objectMap}};
}

CurrencyView RuinView::getCash() const
{
    return CurrencyView{ruin->cash};
}

} // namespace bindings
