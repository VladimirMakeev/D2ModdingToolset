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
static T readSetting(const sol::table& table,
                     const char* name,
                     T def,
                     T min = std::numeric_limits<T>::min(),
                     T max = std::numeric_limits<T>::max())
{
    return std::clamp<T>(table.get_or(name, def), min, max);
}

static void readAiAccuracySettings(Settings& settings, const sol::table& table)
{
    const auto& def = defaultSettings().aiAccuracyBonus;
    auto& aiAccuracy = settings.aiAccuracyBonus;

    auto bonuses = table.get<sol::optional<sol::table>>("aiAccuracyBonus");
    if (!bonuses.has_value()) {
        aiAccuracy = def;
        return;
    }

    aiAccuracy.absolute = readSetting(bonuses.value(), "absolute", def.absolute);
    aiAccuracy.easy = readSetting(bonuses.value(), "easy", def.easy);
    aiAccuracy.average = readSetting(bonuses.value(), "average", def.average);
    aiAccuracy.hard = readSetting(bonuses.value(), "hard", def.hard);
    aiAccuracy.veryHard = readSetting(bonuses.value(), "veryHard", def.veryHard);
}

static void readSettings(Settings& settings, const sol::state& lua)
{
    const sol::table& table = lua["settings"];

    // clang-format off
    settings.unitMaxDamage = readSetting(table, "unitMaxDamage", defaultSettings().unitMaxDamage);
    settings.unitMaxArmor = readSetting(table, "unitMaxArmor", defaultSettings().unitMaxArmor);
    settings.stackScoutRangeMax = readSetting(table, "stackMaxScoutRange", defaultSettings().stackScoutRangeMax);
    settings.shatteredArmorMax = readSetting(table, "shatteredArmorMax", defaultSettings().shatteredArmorMax, 0, baseSettings().shatteredArmorMax);
    settings.shatterDamageMax = readSetting(table, "shatterDamageMax", defaultSettings().shatterDamageMax, 0, baseSettings().shatterDamageMax);
    settings.drainAttackHeal = readSetting(table, "drainAttackHeal", defaultSettings().drainAttackHeal);
    settings.drainOverflowHeal = readSetting(table, "drainOverflowHeal", defaultSettings().drainOverflowHeal);
    settings.carryOverItemsMax = readSetting(table, "carryOverItemsMax", defaultSettings().carryOverItemsMax, 0);
    settings.criticalHitDamage = readSetting(table, "criticalHitDamage", defaultSettings().criticalHitDamage);
    settings.criticalHitChance = readSetting(table, "criticalHitChance", defaultSettings().criticalHitChance, (uint8_t)0, (uint8_t)100);
    settings.mageLeaderAccuracyReduction = readSetting(table, "mageLeaderAccuracyReduction", defaultSettings().mageLeaderAccuracyReduction);
    settings.disableAllowedRoundMax = readSetting(table, "disableAllowedRoundMax", defaultSettings().disableAllowedRoundMax, (uint8_t)1);
    settings.showBanners = readSetting(table, "showBanners", defaultSettings().showBanners);
    settings.showResources = readSetting(table, "showResources", defaultSettings().showResources);
    settings.showLandConverted = readSetting(table, "showLandConverted", defaultSettings().showLandConverted);
    settings.preserveCapitalBuildings = readSetting(table, "preserveCapitalBuildings", defaultSettings().preserveCapitalBuildings);
    settings.allowShatterAttackToMiss = readSetting(table, "allowShatterAttackToMiss", defaultSettings().allowShatterAttackToMiss);
    settings.leveledDoppelgangerAttack = readSetting(table, "leveledDoppelgangerAttack", defaultSettings().leveledDoppelgangerAttack);
    settings.leveledTransformSelfAttack = readSetting(table, "leveledTransformSelfAttack", defaultSettings().leveledTransformSelfAttack);
    settings.leveledSummonAttack = readSetting(table, "leveledSummonAttack", defaultSettings().leveledSummonAttack);
    settings.missChanceSingleRoll = readSetting(table, "missChanceSingleRoll", defaultSettings().missChanceSingleRoll);
    settings.unrestrictedBestowWards = readSetting(table, "unrestrictedBestowWards", defaultSettings().unrestrictedBestowWards);
    settings.debugMode = readSetting(table, "debugHooks", defaultSettings().debugMode);
    // clang-format on

    readAiAccuracySettings(settings, table);
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
        settings.aiAccuracyBonus.absolute = true;
        settings.aiAccuracyBonus.easy = -15;
        settings.aiAccuracyBonus.average = 0;
        settings.aiAccuracyBonus.hard = 5;
        settings.aiAccuracyBonus.veryHard = 10;
        settings.disableAllowedRoundMax = 40;
        settings.showBanners = false;
        settings.showResources = false;
        settings.showLandConverted = false;
        settings.preserveCapitalBuildings = false;
        settings.allowShatterAttackToMiss = false;
        settings.leveledDoppelgangerAttack = false;
        settings.leveledTransformSelfAttack = false;
        settings.leveledSummonAttack = false;
        settings.missChanceSingleRoll = false;
        settings.unrestrictedBestowWards = false;
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
