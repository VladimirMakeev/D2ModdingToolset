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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <cstdint>
#include <filesystem>

namespace hooks {

struct Settings
{
    int unitMaxDamage;
    int unitMaxArmor;
    int stackScoutRangeMax;
    int shatteredArmorMax;
    int shatterDamageMax;
    int drainAttackHeal;
    int drainOverflowHeal;
    int carryOverItemsMax;
    std::uint8_t criticalHitDamage;
    bool showBanners;
    bool showResources;
    bool showLandConverted;
    bool preserveCapitalBuildings;
    bool leveledDoppelgangerAttack;
    bool debugMode;
};

const Settings& baseSettings();
const Settings& defaultSettings();
const Settings& userSettings();

} // namespace hooks

#endif // SETTINGS_H
