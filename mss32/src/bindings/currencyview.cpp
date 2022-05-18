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
    auto view = lua.new_usertype<CurrencyView>(
        "Currency", sol::constructors<CurrencyView(const CurrencyView&)>(),
        sol::meta_function::equal_to, &operator==, sol::meta_function::less_than, &operator<,
        sol::meta_function::less_than_or_equal_to, &operator<=);
    view["infernalMana"] = sol::property(&getInfernalMana, &setInfernalMana);
    view["lifeMana"] = sol::property(&getLifeMana, &setLifeMana);
    view["deathMana"] = sol::property(&getDeathMana, &setDeathMana);
    view["runicMana"] = sol::property(&getRunicMana, &setRunicMana);
    view["groveMana"] = sol::property(&getGroveMana, &setGroveMana);
    view["gold"] = sol::property(&getGold, &setGold);
}

int CurrencyView::getInfernalMana() const
{
    return bank.infernalMana;
}

void CurrencyView::setInfernalMana(int value)
{
    using namespace game;

    BankApi::get().set(&bank, CurrencyType::InfernalMana, (int16_t)value);
}

int CurrencyView::getLifeMana() const
{
    return bank.lifeMana;
}

void CurrencyView::setLifeMana(int value)
{
    using namespace game;

    BankApi::get().set(&bank, CurrencyType::LifeMana, (int16_t)value);
}

int CurrencyView::getDeathMana() const
{
    return bank.deathMana;
}

void CurrencyView::setDeathMana(int value)
{
    using namespace game;

    BankApi::get().set(&bank, CurrencyType::DeathMana, (int16_t)value);
}

int CurrencyView::getRunicMana() const
{
    return bank.runicMana;
}

void CurrencyView::setRunicMana(int value)
{
    using namespace game;

    BankApi::get().set(&bank, CurrencyType::RunicMana, (int16_t)value);
}

int CurrencyView::getGroveMana() const
{
    return bank.groveMana;
}

void CurrencyView::setGroveMana(int value)
{
    using namespace game;

    BankApi::get().set(&bank, CurrencyType::GroveMana, (int16_t)value);
}

int CurrencyView::getGold() const
{
    return bank.gold;
}

void CurrencyView::setGold(int value)
{
    using namespace game;

    BankApi::get().set(&bank, CurrencyType::Gold, (int16_t)value);
}

} // namespace bindings
