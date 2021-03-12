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

#include "settings.h"
#include "log.h"
#include "utils.h"
#include <algorithm>
#include <fmt/format.h>
#include <limits>
#include <lua.hpp>
#include <optional>
#include <sol/sol.hpp>
#include <string>

namespace hooks {

template <typename T>
static T readSetting(const sol::state& lua,
                     const char* name,
                     T def,
                     T min = std::numeric_limits<T>::min(),
                     T max = std::numeric_limits<T>::max())
{
    const std::optional<T> result = lua["settings"][name];
    if (!result) {
        return def;
    }

    return std::clamp<T>(*result, min, max);
}

static void readSettings(Settings& settings, const sol::state& lua)
{
    // clang-format off
    settings.unitMaxDamage = readSetting(lua, "unitMaxDamage", defaultSettings().unitMaxDamage);
    settings.unitMaxArmor = readSetting(lua, "unitMaxArmor", defaultSettings().unitMaxArmor);
    settings.stackScoutRangeMax = readSetting(lua, "stackMaxScoutRange", defaultSettings().stackScoutRangeMax);
    settings.shatteredArmorMax = readSetting(lua, "shatteredArmorMax", defaultSettings().shatteredArmorMax, 0, baseSettings().shatteredArmorMax);
    settings.shatterDamageMax = readSetting(lua, "shatterDamageMax", defaultSettings().shatterDamageMax, 0, baseSettings().shatterDamageMax);
    settings.drainAttackHeal = readSetting(lua, "drainAttackHeal", defaultSettings().drainAttackHeal);
    settings.drainOverflowHeal = readSetting(lua, "drainOverflowHeal", defaultSettings().drainOverflowHeal);
    settings.carryOverItemsMax = readSetting(lua, "carryOverItemsMax", defaultSettings().carryOverItemsMax, 0);
    settings.criticalHitDamage = readSetting(lua, "criticalHitDamage", defaultSettings().criticalHitDamage);
    settings.criticalHitChance = readSetting(lua, "criticalHitChance", defaultSettings().criticalHitChance, (uint8_t)0, (uint8_t)100);
    settings.mageLeaderAccuracyReduction = readSetting(lua, "mageLeaderAccuracyReduction", defaultSettings().mageLeaderAccuracyReduction);
    settings.showBanners = readSetting(lua, "showBanners", defaultSettings().showBanners);
    settings.showResources = readSetting(lua, "showResources", defaultSettings().showResources);
    settings.showLandConverted = readSetting(lua, "showLandConverted", defaultSettings().showLandConverted);
    settings.preserveCapitalBuildings = readSetting(lua, "preserveCapitalBuildings", defaultSettings().preserveCapitalBuildings);
    settings.allowShatterAttackToMiss = readSetting(lua, "allowShatterAttackToMiss", defaultSettings().allowShatterAttackToMiss);
    settings.leveledDoppelgangerAttack = readSetting(lua, "leveledDoppelgangerAttack", defaultSettings().leveledDoppelgangerAttack);
    settings.debugMode = readSetting(lua, "debugHooks", defaultSettings().debugMode);
    // clang-format on
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
        settings.criticalHitChance = 100;
        settings.mageLeaderAccuracyReduction = 10;
        settings.showBanners = false;
        settings.showResources = false;
        settings.showLandConverted = false;
        settings.preserveCapitalBuildings = false;
        settings.allowShatterAttackToMiss = false;
        settings.leveledDoppelgangerAttack = false;
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
        initialized = true;
        settings = defaultSettings();

        const auto settingsPath{hooks::gameFolder() / "Scripts" / "settings.lua"};
        if (std::filesystem::exists(settingsPath)) {
            sol::state lua;

            auto config = lua.load_file(settingsPath.string());
            const auto result = config();

            if (result.valid()) {
                readSettings(settings, lua);
            } else {
                const sol::error err = result;
                hooks::logError("mssProxyError.log",
                                fmt::format("Failed to settings script '{:s}'.\nReason: '{:s}'",
                                            settingsPath.string(), err.what()));
            }
        }
    }

    return settings;
}

} // namespace hooks
