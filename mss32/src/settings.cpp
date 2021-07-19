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
#include "scripts.h"
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

static void readAiAttackPowerSettings(const sol::table& table, Settings::AiAttackPowerBonus& value)
{
    const auto& def = defaultSettings().aiAttackPowerBonus;

    auto bonuses = table.get<sol::optional<sol::table>>("aiAccuracyBonus");
    if (!bonuses.has_value()) {
        value = def;
        return;
    }

    value.absolute = readSetting(bonuses.value(), "absolute", def.absolute);
    value.easy = readSetting(bonuses.value(), "easy", def.easy);
    value.average = readSetting(bonuses.value(), "average", def.average);
    value.hard = readSetting(bonuses.value(), "hard", def.hard);
    value.veryHard = readSetting(bonuses.value(), "veryHard", def.veryHard);
}

static Color readColor(const sol::table& table, const Color& def)
{
    Color color{};
    color.r = readSetting(table, "red", def.r);
    color.g = readSetting(table, "green", def.g);
    color.b = readSetting(table, "blue", def.b);

    return color;
}

static void readMovementCostSettings(const sol::table& table, Settings::MovementCost& value)
{
    const auto& defTextColor = defaultSettings().movementCost.textColor;
    const auto& defOutlineColor = defaultSettings().movementCost.outlineColor;

    value.show = defaultSettings().movementCost.show;
    value.textColor = defTextColor;
    value.outlineColor = defOutlineColor;

    auto moveCost = table.get<sol::optional<sol::table>>("movementCost");
    if (!moveCost.has_value()) {
        return;
    }

    value.show = readSetting(moveCost.value(), "show", defaultSettings().movementCost.show);

    auto textColor = moveCost.value().get<sol::optional<sol::table>>("textColor");
    if (textColor.has_value()) {
        value.textColor = readColor(textColor.value(), defTextColor);
    }

    auto outlineColor = moveCost.value().get<sol::optional<sol::table>>("outlineColor");
    if (outlineColor.has_value()) {
        value.outlineColor = readColor(outlineColor.value(), defOutlineColor);
    }
}

static void readSettings(const sol::table& table, Settings& settings)
{
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
    settings.mageLeaderAttackPowerReduction = readSetting(table, "mageLeaderAccuracyReduction", defaultSettings().mageLeaderAttackPowerReduction);
    settings.disableAllowedRoundMax = readSetting(table, "disableAllowedRoundMax", defaultSettings().disableAllowedRoundMax, (uint8_t)1);
    settings.shatterDamageUpgradeRatio = readSetting(table, "shatterDamageUpgradeRatio", defaultSettings().shatterDamageUpgradeRatio);
    settings.showBanners = readSetting(table, "showBanners", defaultSettings().showBanners);
    settings.showResources = readSetting(table, "showResources", defaultSettings().showResources);
    settings.showLandConverted = readSetting(table, "showLandConverted", defaultSettings().showLandConverted);
    settings.preserveCapitalBuildings = readSetting(table, "preserveCapitalBuildings", defaultSettings().preserveCapitalBuildings);
    settings.allowShatterAttackToMiss = readSetting(table, "allowShatterAttackToMiss", defaultSettings().allowShatterAttackToMiss);
    settings.doppelgangerRespectsEnemyImmunity = readSetting(table, "doppelgangerRespectsEnemyImmunity", defaultSettings().doppelgangerRespectsEnemyImmunity);
    settings.doppelgangerRespectsAllyImmunity = readSetting(table, "doppelgangerRespectsAllyImmunity", defaultSettings().doppelgangerRespectsAllyImmunity);
    settings.leveledDoppelgangerAttack = readSetting(table, "leveledDoppelgangerAttack", defaultSettings().leveledDoppelgangerAttack);
    settings.leveledTransformSelfAttack = readSetting(table, "leveledTransformSelfAttack", defaultSettings().leveledTransformSelfAttack);
    settings.leveledSummonAttack = readSetting(table, "leveledSummonAttack", defaultSettings().leveledSummonAttack);
    settings.missChanceSingleRoll = readSetting(table, "missChanceSingleRoll", defaultSettings().missChanceSingleRoll);
    settings.unrestrictedBestowWards = readSetting(table, "unrestrictedBestowWards", defaultSettings().unrestrictedBestowWards);
    settings.freeTransformSelfAttack = readSetting(table, "freeTransformSelfAttack", defaultSettings().freeTransformSelfAttack);
    settings.detailedAttackDescription = readSetting(table, "detailedAttackDescription", defaultSettings().detailedAttackDescription);
    settings.debugMode = readSetting(table, "debugHooks", defaultSettings().debugMode);
    // clang-format on

    readAiAttackPowerSettings(table, settings.aiAttackPowerBonus);
    readMovementCostSettings(table, settings.movementCost);
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
        settings.mageLeaderAttackPowerReduction = 10;
        settings.aiAttackPowerBonus.absolute = true;
        settings.aiAttackPowerBonus.easy = -15;
        settings.aiAttackPowerBonus.average = 0;
        settings.aiAttackPowerBonus.hard = 5;
        settings.aiAttackPowerBonus.veryHard = 10;
        settings.disableAllowedRoundMax = 40;
        settings.shatterDamageUpgradeRatio = 100;
        settings.showBanners = false;
        settings.showResources = false;
        settings.showLandConverted = false;
        settings.preserveCapitalBuildings = false;
        settings.allowShatterAttackToMiss = false;
        settings.doppelgangerRespectsEnemyImmunity = false;
        settings.doppelgangerRespectsAllyImmunity = false;
        settings.leveledDoppelgangerAttack = false;
        settings.leveledTransformSelfAttack = false;
        settings.leveledSummonAttack = false;
        settings.missChanceSingleRoll = false;
        settings.unrestrictedBestowWards = false;
        settings.freeTransformSelfAttack = false;
        settings.detailedAttackDescription = false;
        settings.movementCost.textColor = Color{200, 200, 200};
        settings.movementCost.show = false;
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
        settings.movementCost.show = true;
        settings.detailedAttackDescription = true;

        initialized = true;
    }

    return settings;
}

void initializeUserSettings(Settings& value)
{
    value = defaultSettings();

    const auto path{scriptsFolder() / "settings.lua"};
    try {
        const auto lua{loadScriptFile(path)};
        if (!lua)
            return;

        const sol::table& table = (*lua)["settings"];
        readSettings(table, value);
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to read script '{:s}'.\n"
                                        "Reason: '{:s}'",
                                        path.string(), e.what()));
    }
}

const Settings& userSettings()
{
    static Settings settings;
    static bool initialized = false;

    if (!initialized) {
        initializeUserSettings(settings);
        initialized = true;
    }

    return settings;
}

} // namespace hooks
