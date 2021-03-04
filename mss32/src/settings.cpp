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

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "settings.h"
#include "utils.h"
#include <Windows.h>
#include <algorithm>
#include <fmt/format.h>
#include <limits>
#include <string>

namespace hooks {

template <typename T>
T readNumberSetting(const std::string& iniPath,
                    const char* key,
                    T def,
                    T min = std::numeric_limits<T>::min(),
                    T max = std::numeric_limits<T>::max())
{
    auto value = GetPrivateProfileInt("Disciple", key, def, iniPath.c_str());
    return std::clamp<T>(value, min, max);
}

bool readBooleanSetting(const std::string& iniPath, const char* key, bool def)
{
    return readNumberSetting<int>(iniPath, key, def, 0, 1) != 0;
}

const Settings& baseSettings()
{
    static Settings settings;
    static bool initialized = false;

    if (!initialized) {
        settings.unitMaxDamage = 300;
        settings.unitMaxArmor = 90;
        settings.stackScoutRangeMax = 8;
        settings.shatteredArmorMax = 100;
        settings.shatterDamageMax = 100;
        settings.drainAttackHeal = 50;
        settings.drainOverflowHeal = 50;
        settings.carryOverItemsMax = 5;
        settings.criticalHitDamage = 5;
        settings.mageLeaderAccuracyReduction = 10;
        settings.showBanners = false;
        settings.showResources = false;
        settings.showLandConverted = false;
        settings.preserveCapitalBuildings = false;
        settings.debugMode = false;

        initialized = true;
    }

    return settings;
}

const Settings& defaultSettings()
{
    static Settings settings;
    static bool initialized = false;

    if (!initialized) {
        settings = baseSettings();
        settings.showBanners = true;
        settings.showResources = true;

        initialized = true;
    }

    return settings;
}

const Settings& userSettings()
{
    static Settings settings;
    static bool initialized = false;

    if (!initialized) {
        const auto iniPath = (hooks::gameFolder() / "disciple.ini").string();

        // clang-format off
        settings.unitMaxDamage = readNumberSetting(iniPath, "UnitMaxDamage", defaultSettings().unitMaxDamage);
        settings.unitMaxArmor = readNumberSetting(iniPath, "UnitMaxArmor", defaultSettings().unitMaxArmor);
        settings.stackScoutRangeMax = readNumberSetting(iniPath, "StackMaxScoutRange", defaultSettings().stackScoutRangeMax);
        settings.shatteredArmorMax = readNumberSetting(iniPath, "ShatteredArmorMax", defaultSettings().shatteredArmorMax, 0, baseSettings().shatteredArmorMax);
        settings.shatterDamageMax = readNumberSetting(iniPath, "ShatterDamageMax", defaultSettings().shatterDamageMax, 0, baseSettings().shatterDamageMax);
        settings.drainAttackHeal = readNumberSetting(iniPath, "DrainAttackHeal", defaultSettings().drainAttackHeal);
        settings.drainOverflowHeal = readNumberSetting(iniPath, "DrainOverflowHeal", defaultSettings().drainOverflowHeal);
        settings.carryOverItemsMax = readNumberSetting(iniPath, "CarryOverItemsMax", defaultSettings().carryOverItemsMax, 0);
        settings.criticalHitDamage = readNumberSetting(iniPath, "CriticalHitDamage", defaultSettings().criticalHitDamage);
        settings.mageLeaderAccuracyReduction = readNumberSetting(iniPath, "MageLeaderAccuracyReduction", defaultSettings().mageLeaderAccuracyReduction);
        settings.showBanners = readBooleanSetting(iniPath, "ShowBanners", defaultSettings().showBanners);
        settings.showResources = readBooleanSetting(iniPath, "ShowResources", defaultSettings().showResources);
        settings.showLandConverted = readBooleanSetting(iniPath, "ShowLandConverted", defaultSettings().showLandConverted);
        settings.preserveCapitalBuildings = readBooleanSetting(iniPath, "PreserveCapitalBuildings", defaultSettings().preserveCapitalBuildings);
        settings.debugMode = readBooleanSetting(iniPath, "DebugHooks", defaultSettings().debugMode);
        // clang-format on

        initialized = true;
    }

    return settings;
}

} // namespace hooks
