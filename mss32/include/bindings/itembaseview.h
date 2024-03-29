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

#ifndef ITEMBASEVIEW_H
#define ITEMBASEVIEW_H

#include "currencyview.h"
#include "idview.h"
#include <optional>

namespace sol {
class state;
}

namespace game {
struct CItemBase;
} // namespace game

namespace bindings {

class UnitImplView;
class AttackView;

class ItemBaseView
{
public:
    ItemBaseView(const game::CItemBase* item);

    static void bind(sol::state& lua);

    IdView getId() const;
    int getCategory() const;
    CurrencyView getValue() const;
    std::optional<UnitImplView> getUnitImpl() const;
    std::optional<AttackView> getAttack() const;

private:
    const game::CItemBase* item;
};

} // namespace bindings

#endif // ITEMBASEVIEW_H
