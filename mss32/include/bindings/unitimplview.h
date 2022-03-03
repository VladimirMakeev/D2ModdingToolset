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

#ifndef UNITIMPLVIEW_H
#define UNITIMPLVIEW_H

#include <optional>

namespace sol {
class state;
}

namespace game {
struct IUsUnit;
}

namespace bindings {

class DynUpgradeView;
class AttackView;

class UnitImplView
{
public:
    UnitImplView(game::IUsUnit* unitImpl);

    static void bind(sol::state& lua);

    /** Returns unit implementation level. */
    int getLevel() const;
    /** Returns experience points needed for next level. */
    int getXpNext() const;
    /** Returns level after which dynUpgrade2 rules are applied. */
    int getDynUpgLevel() const;
    int getXpKilled() const;
    int getArmor() const;
    int getRegen() const;
    int getRace() const;
    int getSubRace() const;
    bool isSmall() const;
    bool isMale() const;
    bool isWaterOnly() const;
    bool attacksTwice() const;

    /** Returns dynamic upgrade 1. */
    std::optional<DynUpgradeView> getDynUpgrade1() const;
    /** Returns dynamic upgrade 2. */
    std::optional<DynUpgradeView> getDynUpgrade2() const;
    /** Returns primary attack. */
    std::optional<AttackView> getAttack() const;
    /** Returns secondary attack. */
    std::optional<AttackView> getAttack2() const;

protected:
    std::optional<DynUpgradeView> getDynUpgrade(int upgradeNumber) const;

private:
    game::IUsUnit* impl;
};

} // namespace bindings

#endif // UNITIMPLVIEW_H
