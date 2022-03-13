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

#ifndef CURRENCYVIEW_H
#define CURRENCYVIEW_H

#include "currency.h"
#include <ostream>

namespace sol {
class state;
}

namespace bindings {

class CurrencyView
{
public:
    CurrencyView(const game::Bank& value);
    bool operator==(const CurrencyView& value) const;
    bool operator<(const CurrencyView& value) const;
    bool operator<=(const CurrencyView& value) const;

    static void bind(sol::state& lua);

    int getInfernalMana() const;
    int getLifeMana() const;
    int getDeathMana() const;
    int getRunicMana() const;
    int getGroveMana() const;
    int getGold() const;

    game::Bank bank;
};

/** Overloaded operator allows calling 'tostring()' with CurrencyView in lua scripts. */
static inline std::ostream& operator<<(std::ostream& os, const CurrencyView& value)
{
    char buffer[36] = {0};
    os << game::BankApi::get().toString(&value.bank, buffer);
    return os;
}

} // namespace bindings

#endif // CURRENCYVIEW_H
