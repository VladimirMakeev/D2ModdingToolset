/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#ifndef DYNUPGRADEVIEW_H
#define DYNUPGRADEVIEW_H

#include "currencyview.h"
#include "idview.h"

namespace sol {
class state;
}

namespace game {
struct CDynUpgrade;
}

namespace bindings {

class DynUpgradeView
{
public:
    DynUpgradeView(const game::CDynUpgrade* upgrade);

    static void bind(sol::state& lua);

    IdView getId() const;
    CurrencyView getEnrollCost() const;
    int getHp() const;
    int getArmor() const;
    int getRegen() const;
    CurrencyView getReviveCost() const;
    CurrencyView getHealCost() const;
    CurrencyView getTrainingCost() const;
    int getXpKilled() const;
    int getXpNext() const;
    int getMove() const;
    int getNegotiate() const;
    int getInitiative() const;
    int getPower() const;
    int getDamage() const;
    int getHeal() const;

private:
    const game::CDynUpgrade* upgrade;
};

} // namespace bindings

#endif // DYNUPGRADEVIEW_H
