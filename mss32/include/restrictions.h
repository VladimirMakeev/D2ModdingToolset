/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef RESTRICTIONS_H
#define RESTRICTIONS_H

#include <cstdint>

namespace game {

template <typename T>
struct Restriction
{
    T min;
    T max;
};

/** Determines a number of unit over-levels to display colored swords on top of its icon. */
typedef std::uint32_t UnitHighLevels[3];

/** Addresses of global variables representing game restrictions. */
struct GameRestrictions
{
    Restriction<int>* spellLevel;
    Restriction<int>* unitLevel;
    Restriction<int>* attackDamage;
    Restriction<int>* unitArmor;
    Restriction<int>* unitHp;
    Restriction<int>* stackScoutRange;
    Restriction<int>* stackMovement;
    Restriction<int>* stackLeadership;
    Restriction<int>* attackInitiative;
    Restriction<int>* attackPower;
    std::uint32_t* fighterExplorerLeaderBonusMaxDamage;
    std::uint8_t* criticalHitDamage;
    std::uint8_t* mageLeaderAttackPowerReduction;
    UnitHighLevels* unitHighLevels;

    // No dedicated game variable for this, see implementation of CDBTableApi::readHeal
    Restriction<int> attackHeal;
    // No dedicated game variable for this, see implementation of DBReadRegenFieldWithBoundsCheck
    // (Akella 0x5A7EB0)
    Restriction<int> unitRegen;
    // No dedicated game variable for this, see implementation of DBReadPercentFieldWithBoundsCheck
    // (Akella 0x596A86)
    Restriction<int> percentValue;
};

GameRestrictions& gameRestrictions();

} // namespace game

#endif // RESTRICTIONS_H
