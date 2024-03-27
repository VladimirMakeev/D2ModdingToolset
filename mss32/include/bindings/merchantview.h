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

#ifndef MERCHANTVIEW_H
#define MERCHANTVIEW_H

#include "itembaseview.h"
#include "siteview.h"

namespace game {
struct CMidSiteMerchant;
}

namespace bindings {

class MerchantItemView
{
public:
    MerchantItemView(const game::CMidgardID& globalItemId, int amount);

    static void bind(sol::state& lua);

    ItemBaseView getItemBase() const;
    int getAmount();

private:
    game::CMidgardID globalItemId;
    int amount;
};

class MerchantView : public SiteView
{
public:
    MerchantView(const game::CMidSiteMerchant* merchant, const game::IMidgardObjectMap* objectMap);

    static void bind(sol::state& lua);

    std::vector<MerchantItemView> getItems() const;
    bool isMission() const;
};

} // namespace bindings

#endif // MERCHANTVIEW_H
