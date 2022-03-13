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

#include "currencyview.h"
#include <sol/sol.hpp>

namespace bindings {

CurrencyView::CurrencyView(const game::Bank& value)
    : bank(value)
{ }

bool CurrencyView::operator==(const CurrencyView& value) const
{
    return bank == value.bank;
}

bool CurrencyView::operator<(const CurrencyView& value) const
{
    return bank < value.bank;
}

bool CurrencyView::operator<=(const CurrencyView& value) const
{
    return bank <= value.bank;
}

void CurrencyView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<CurrencyView>("CurrencyView", sol::meta_function::equal_to,
                                               &operator==, sol::meta_function::less_than,
                                               &operator<,
                                               sol::meta_function::less_than_or_equal_to,
                                               &operator<=);
    view["infernalMana"] = sol::property(&getInfernalMana);
    view["lifeMana"] = sol::property(&getLifeMana);
    view["deathMana"] = sol::property(&getDeathMana);
    view["runicMana"] = sol::property(&getRunicMana);
    view["groveMana"] = sol::property(&getGroveMana);
    view["gold"] = sol::property(&getGold);
}

int CurrencyView::getInfernalMana() const
{
    return bank.infernalMana;
}

int CurrencyView::getLifeMana() const
{
    return bank.lifeMana;
}

int CurrencyView::getDeathMana() const
{
    return bank.deathMana;
}

int CurrencyView::getRunicMana() const
{
    return bank.runicMana;
}

int CurrencyView::getGroveMana() const
{
    return bank.groveMana;
}

int CurrencyView::getGold() const
{
    return bank.gold;
}

} // namespace bindings
