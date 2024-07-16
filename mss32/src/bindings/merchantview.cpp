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

#include "merchantview.h"
#include "globaldata.h"
#include "itemutils.h"
#include "midsitemerchant.h"
#include <sol/sol.hpp>

namespace bindings {

MerchantItemView::MerchantItemView(const game::CMidgardID& globalItemId, int amount)
    : globalItemId{globalItemId}
    , amount{amount}
{ }

void MerchantItemView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<MerchantItemView>("MerchantItemView");

    view["base"] = sol::property(&MerchantItemView::getItemBase);
    view["amount"] = sol::property(&MerchantItemView::getAmount);
}

ItemBaseView MerchantItemView::getItemBase() const
{
    using namespace game;

    const auto& global = GlobalDataApi::get();
    auto* globalData = *global.getGlobalData();

    const auto* globalItem = global.findItemById(globalData->itemTypes, &globalItemId);
    return ItemBaseView{globalItem};
}

int MerchantItemView::getAmount()
{
    return amount;
}

MerchantView::MerchantView(const game::CMidSiteMerchant* merchant,
                           const game::IMidgardObjectMap* objectMap)
    : SiteView(merchant, objectMap)
{ }

void MerchantView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<MerchantView>("MerchantView", sol::base_classes,
                                               sol::bases<SiteView>());
    bindAccessMethods(view);

    view["items"] = sol::property(&MerchantView::getItems);
    view["temple"] = sol::property(&MerchantView::isMission);
}

std::vector<MerchantItemView> MerchantView::getItems() const
{
    const auto* merchant = static_cast<const game::CMidSiteMerchant*>(site);

    std::vector<MerchantItemView> items;
    items.reserve(merchant->items.length);

    for (const auto& [itemId, amount] : merchant->items) {
        items.emplace_back(itemId, amount);
    }

    return items;
}

bool MerchantView::isMission() const
{
    const auto* merchant = static_cast<const game::CMidSiteMerchant*>(site);
    return merchant->mission;
}

} // namespace bindings
