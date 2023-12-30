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

#include "dynupgradeview.h"
#include "dynupgrade.h"
#include <sol/sol.hpp>

namespace bindings {

DynUpgradeView::DynUpgradeView(const game::CDynUpgrade* upgrade)
    : upgrade(upgrade)
{ }

void DynUpgradeView::bind(sol::state& lua)
{
    auto impl = lua.new_usertype<DynUpgradeView>("DynUpgrade");
    impl["id"] = sol::property(&DynUpgradeView::getId);
    impl["enrollCost"] = sol::property(&DynUpgradeView::getEnrollCost);
    impl["hp"] = sol::property(&DynUpgradeView::getHp);
    impl["armor"] = sol::property(&DynUpgradeView::getArmor);
    impl["regen"] = sol::property(&DynUpgradeView::getRegen);
    impl["reviveCost"] = sol::property(&DynUpgradeView::getReviveCost);
    impl["healCost"] = sol::property(&DynUpgradeView::getHealCost);
    impl["trainingCost"] = sol::property(&DynUpgradeView::getTrainingCost);
    impl["xpKilled"] = sol::property(&DynUpgradeView::getXpKilled);
    impl["xpNext"] = sol::property(&DynUpgradeView::getXpNext);
    impl["move"] = sol::property(&DynUpgradeView::getMove);
    impl["negotiate"] = sol::property(&DynUpgradeView::getNegotiate);
    impl["initiative"] = sol::property(&DynUpgradeView::getInitiative);
    impl["power"] = sol::property(&DynUpgradeView::getPower);
    impl["damage"] = sol::property(&DynUpgradeView::getDamage);
    impl["heal"] = sol::property(&DynUpgradeView::getHeal);
}

IdView DynUpgradeView::getId() const
{
    return upgrade->upgradeId;
}

CurrencyView DynUpgradeView::getEnrollCost() const
{
    return upgrade->enrollCost;
}

int DynUpgradeView::getHp() const
{
    return upgrade->hp;
}

int DynUpgradeView::getArmor() const
{
    return upgrade->armor;
}

int DynUpgradeView::getRegen() const
{
    return upgrade->regen;
}

CurrencyView DynUpgradeView::getReviveCost() const
{
    return upgrade->reviveCost;
}

CurrencyView DynUpgradeView::getHealCost() const
{
    return upgrade->healCost;
}

CurrencyView DynUpgradeView::getTrainingCost() const
{
    return upgrade->trainingCost;
}

int DynUpgradeView::getXpKilled() const
{
    return upgrade->xpKilled;
}

int DynUpgradeView::getXpNext() const
{
    return upgrade->xpNext;
}

int DynUpgradeView::getMove() const
{
    return upgrade->move;
}

int DynUpgradeView::getNegotiate() const
{
    return upgrade->negotiate;
}

int DynUpgradeView::getInitiative() const
{
    return upgrade->initiative;
}

int DynUpgradeView::getPower() const
{
    return upgrade->power;
}

int DynUpgradeView::getDamage() const
{
    return upgrade->damage;
}

int DynUpgradeView::getHeal() const
{
    return upgrade->heal;
}

} // namespace bindings
