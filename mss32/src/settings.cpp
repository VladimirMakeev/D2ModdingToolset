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

static std::string readSetting(const sol::table& table, const char* name, const std::string& def)
{
    return table.get_or(name, def);
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

static void readAllowBattleItemsSettings(const sol::table& table, Settings::AllowBattleItems& value)
{
    const auto& def = defaultSettings().allowBattleItems;

    auto category = table.get<sol::optional<sol::table>>("allowBattleItems");
    if (!category.has_value()) {
        value = def;
        return;
    }

    value.onTransformOther = readSetting(category.value(), "onTransformOther",
                                         def.onTransformOther);
    value.onTransformSelf = readSetting(category.value(), "onTransformSelf", def.onTransformSelf);
    value.onDrainLevel = readSetting(category.value(), "onDrainLevel", def.onDrainLevel);
    value.onDoppelganger = readSetting(category.value(), "onDoppelganger", def.onDoppelganger);
}

static void readUnitEncyclopediaSettings(const sol::table& table, Settings::UnitEncyclopedia& value)
{
    const auto& def = defaultSettings().unitEncyclopedia;

    auto category = table.get<sol::optional<sol::table>>("unitEncyclopedia");
    if (!category.has_value()) {
        value = def;
        // For backward compatibility
        value.detailedAttackDescription = readSetting(table, "detailedAttackDescription",
                                                      def.detailedAttackDescription);
        return;
    }

    value.detailedUnitDescription = readSetting(category.value(), "detailedUnitDescription",
                                                def.detailedUnitDescription);
    value.detailedAttackDescription = readSetting(category.value(), "detailedAttackDescription",
                                                  def.detailedAttackDescription);
    value.displayDynamicUpgradeValues = readSetting(category.value(), "displayDynamicUpgradeValues",
                                                    def.displayDynamicUpgradeValues);
    value.displayBonusHp = readSetting(category.value(), "displayBonusHp", def.displayBonusHp);
    value.displayBonusXp = readSetting(category.value(), "displayBonusXp", def.displayBonusXp);
    value.displayInfiniteAttackIndicator = readSetting(category.value(),
                                                       "displayInfiniteAttackIndicator",
                                                       def.displayInfiniteAttackIndicator);
    value.displayCriticalHitTextInAttackName = readSetting(category.value(),
                                                           "displayCriticalHitTextInAttackName",
                                                           def.displayCriticalHitTextInAttackName);
    value.updateOnShiftKeyPress = readSetting(category.value(), "updateOnShiftKeyPress",
                                              def.updateOnShiftKeyPress);
    value.updateOnCtrlKeyPress = readSetting(category.value(), "updateOnCtrlKeyPress",
                                             def.updateOnCtrlKeyPress);
    value.updateOnAltKeyPress = readSetting(category.value(), "updateOnAltKeyPress",
                                            def.updateOnAltKeyPress);
}

static void readModifierSettings(const sol::table& table, Settings::Modifiers& value)
{
    const auto& def = defaultSettings().modifiers;

    auto category = table.get<sol::optional<sol::table>>("modifiers");
    if (!category.has_value()) {
        value = def;
        return;
    }

    value.cumulativeUnitRegeneration = readSetting(category.value(), "cumulativeUnitRegeneration",
                                                   def.cumulativeUnitRegeneration);
    value.notifyModifiersChanged = readSetting(category.value(), "notifyModifiersChanged",
                                               def.notifyModifiersChanged);
    value.validateUnitsOnGroupChanged = readSetting(category.value(), "validateUnitsOnGroupChanged",
                                                    def.validateUnitsOnGroupChanged);
}

static Color readColor(const sol::table& table, const Color& def)
{
    Color color{};
    color.r = readSetting(table, "red", def.r);
    color.g = readSetting(table, "green", def.g);
    color.b = readSetting(table, "blue", def.b);

    return color;
}

static void readWaterMoveCostSettings(const sol::table& table, Settings::MovementCost::Water& water)
{
    const auto& def = defaultSettings().movementCost.water;

    water.dflt = readSetting(table, "default", def.dflt, 1);
    water.deadLeader = readSetting(table, "withDeadLeader", def.deadLeader, 1);
    water.withBonus = readSetting(table, "withBonus", def.withBonus, 1);
    water.waterOnly = readSetting(table, "waterOnly", def.waterOnly, 1);
}

static void readForestMoveCostSettings(const sol::table& table,
                                       Settings::MovementCost::Forest& forest)
{
    const auto& def = defaultSettings().movementCost.forest;

    forest.dflt = readSetting(table, "default", def.dflt, 1);
    forest.deadLeader = readSetting(table, "withDeadLeader", def.deadLeader, 1);
    forest.withBonus = readSetting(table, "withBonus", def.withBonus, 1);
}

static void readPlainMoveCostSettings(const sol::table& table, Settings::MovementCost::Plain& plain)
{
    const auto& def = defaultSettings().movementCost.plain;

    plain.dflt = readSetting(table, "default", def.dflt, 1);
    plain.deadLeader = readSetting(table, "withDeadLeader", def.deadLeader, 1);
    plain.onRoad = readSetting(table, "onRoad", def.onRoad, 1);
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

    auto water = moveCost.value().get<sol::optional<sol::table>>("water");
    if (water.has_value()) {
        readWaterMoveCostSettings(water.value(), value.water);
    }

    auto forest = moveCost.value().get<sol::optional<sol::table>>("forest");
    if (forest.has_value()) {
        readForestMoveCostSettings(forest.value(), value.forest);
    }

    auto plain = moveCost.value().get<sol::optional<sol::table>>("plain");
    if (plain.has_value()) {
        readPlainMoveCostSettings(plain.value(), value.plain);
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

static void readLobbySettings(const sol::table& table, Settings::Lobby& value)
{
    const auto& settings = defaultSettings().lobby;

    value.server.ip = settings.server.ip;
    value.server.port = settings.server.port;
    value.client.port = settings.client.port;

    auto lobby = table.get<sol::optional<sol::table>>("lobby");
    if (!lobby.has_value()) {
        return;
    }

    auto server = lobby.value().get<sol::optional<sol::table>>("server");
    if (server.has_value()) {
        value.server.ip = readSetting(server.value(), "ip", settings.server.ip);
        value.server.port = readSetting(server.value(), "port", settings.server.port);
    }

    auto client = lobby.value().get<sol::optional<sol::table>>("client");
    if (client.has_value()) {
        value.client.port = readSetting(client.value(), "port", settings.client.port);
    }
}

static void readDebugSettings(const sol::table& table, Settings::Debug& value)
{
    const auto& def = defaultSettings().debug;

    // 'debug' is reserved to Lua standard debug library
    auto category = table.get<sol::optional<sol::table>>("debugging");
    if (!category.has_value()) {
        value = def;
        return;
    }

    value.sendObjectsChangesTreshold = readSetting(category.value(), "sendObjectsChangesTreshold",
                                                   def.sendObjectsChangesTreshold);
    value.logSinglePlayerMessages = readSetting(category.value(), "logSinglePlayerMessages",
                                                def.logSinglePlayerMessages);
}

static void readEngineSettings(const sol::table& table, Settings::Engine& value)
{
    const auto& def = defaultSettings().engine;

    auto category = table.get<sol::optional<sol::table>>("engine");
    if (!category.has_value()) {
        value = def;
        return;
    }

    value.sendRefreshInfoObjectCountLimit = readSetting(category.value(),
                                                        "sendRefreshInfoObjectCountLimit",
                                                        def.sendRefreshInfoObjectCountLimit);
}

static void readBattleSettings(const sol::table& table, Settings::Battle& value)
{
    const auto& def = defaultSettings().battle;

    auto category = table.get<sol::optional<sol::table>>("battle");
    if (!category.has_value()) {
        value = def;
        return;
    }

    value.allowRetreatedUnitsToUpgrade = readSetting(category.value(),
                                                     "allowRetreatedUnitsToUpgrade",
                                                     def.allowRetreatedUnitsToUpgrade);
    value.carryXpOverUpgrade = readSetting(category.value(), "carryXpOverUpgrade",
                                           def.carryXpOverUpgrade);
    value.allowMultiUpgrade = readSetting(category.value(), "allowMultiUpgrade",
                                          def.allowMultiUpgrade);
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
    settings.splitDamageMultiplier = readSetting(table, "splitDamageMultiplier", defaultSettings().splitDamageMultiplier, (uint8_t)1, (uint8_t)6);
    settings.showBanners = readSetting(table, "showBanners", defaultSettings().showBanners);
    settings.showResources = readSetting(table, "showResources", defaultSettings().showResources);
    settings.showLandConverted = readSetting(table, "showLandConverted", defaultSettings().showLandConverted);
    settings.preserveCapitalBuildings = readSetting(table, "preserveCapitalBuildings", defaultSettings().preserveCapitalBuildings);
    settings.buildTempleForWarriorLord = readSetting(table, "buildTempleForWarriorLord", defaultSettings().buildTempleForWarriorLord);
    settings.allowShatterAttackToMiss = readSetting(table, "allowShatterAttackToMiss", defaultSettings().allowShatterAttackToMiss);
    settings.doppelgangerRespectsEnemyImmunity = readSetting(table, "doppelgangerRespectsEnemyImmunity", defaultSettings().doppelgangerRespectsEnemyImmunity);
    settings.doppelgangerRespectsAllyImmunity = readSetting(table, "doppelgangerRespectsAllyImmunity", defaultSettings().doppelgangerRespectsAllyImmunity);
    settings.leveledDoppelgangerAttack = readSetting(table, "leveledDoppelgangerAttack", defaultSettings().leveledDoppelgangerAttack);
    settings.leveledTransformSelfAttack = readSetting(table, "leveledTransformSelfAttack", defaultSettings().leveledTransformSelfAttack);
    settings.leveledTransformOtherAttack = readSetting(table, "leveledTransformOtherAttack", defaultSettings().leveledTransformOtherAttack);
    settings.leveledDrainLevelAttack = readSetting(table, "leveledDrainLevelAttack", defaultSettings().leveledDrainLevelAttack);
    settings.leveledSummonAttack = readSetting(table, "leveledSummonAttack", defaultSettings().leveledSummonAttack);
    settings.missChanceSingleRoll = readSetting(table, "missChanceSingleRoll", defaultSettings().missChanceSingleRoll);
    settings.unrestrictedBestowWards = readSetting(table, "unrestrictedBestowWards", defaultSettings().unrestrictedBestowWards);
    settings.freeTransformSelfAttack = readSetting(table, "freeTransformSelfAttack", defaultSettings().freeTransformSelfAttack);
    settings.freeTransformSelfAttackInfinite = readSetting(table, "freeTransformSelfAttackInfinite", defaultSettings().freeTransformSelfAttackInfinite);
    settings.fixEffectiveHpFormula = readSetting(table, "fixEffectiveHpFormula", defaultSettings().fixEffectiveHpFormula);
    settings.debugMode = readSetting(table, "debugHooks", defaultSettings().debugMode);
    // clang-format on

    readAiAttackPowerSettings(table, settings.aiAttackPowerBonus);
    readAllowBattleItemsSettings(table, settings.allowBattleItems);
    readUnitEncyclopediaSettings(table, settings.unitEncyclopedia);
    readModifierSettings(table, settings.modifiers);
    readMovementCostSettings(table, settings.movementCost);
    readLobbySettings(table, settings.lobby);
    readDebugSettings(table, settings.debug);
    readEngineSettings(table, settings.engine);
    readBattleSettings(table, settings.battle);
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
        settings.splitDamageMultiplier = 1;
        settings.showBanners = false;
        settings.showResources = false;
        settings.showLandConverted = false;
        settings.preserveCapitalBuildings = false;
        settings.buildTempleForWarriorLord = false;
        settings.allowShatterAttackToMiss = false;
        settings.doppelgangerRespectsEnemyImmunity = false;
        settings.doppelgangerRespectsAllyImmunity = false;
        settings.leveledDoppelgangerAttack = false;
        settings.leveledTransformSelfAttack = false;
        settings.leveledTransformOtherAttack = false;
        settings.leveledDrainLevelAttack = false;
        settings.leveledSummonAttack = false;
        settings.missChanceSingleRoll = false;
        settings.unrestrictedBestowWards = false;
        settings.freeTransformSelfAttack = false;
        settings.freeTransformSelfAttackInfinite = false;
        settings.unitEncyclopedia.detailedUnitDescription = false;
        settings.unitEncyclopedia.detailedAttackDescription = false;
        settings.unitEncyclopedia.displayDynamicUpgradeValues = false;
        settings.unitEncyclopedia.displayBonusHp = false;
        settings.unitEncyclopedia.displayBonusXp = false;
        settings.unitEncyclopedia.displayInfiniteAttackIndicator = false;
        settings.unitEncyclopedia.displayCriticalHitTextInAttackName = false;
        settings.unitEncyclopedia.updateOnShiftKeyPress = false;
        settings.unitEncyclopedia.updateOnCtrlKeyPress = false;
        settings.unitEncyclopedia.updateOnAltKeyPress = false;
        settings.fixEffectiveHpFormula = false;
        settings.modifiers.cumulativeUnitRegeneration = false;
        settings.modifiers.notifyModifiersChanged = false;
        settings.modifiers.validateUnitsOnGroupChanged = false;
        settings.allowBattleItems.onTransformOther = false;
        settings.allowBattleItems.onTransformSelf = false;
        settings.allowBattleItems.onDrainLevel = false;
        settings.allowBattleItems.onDoppelganger = false;
        settings.movementCost.water.dflt = 6;
        settings.movementCost.water.deadLeader = 12;
        settings.movementCost.water.withBonus = 2;
        settings.movementCost.water.waterOnly = 2;
        settings.movementCost.forest.dflt = 4;
        settings.movementCost.forest.deadLeader = 8;
        settings.movementCost.forest.withBonus = 2;
        settings.movementCost.plain.dflt = 2;
        settings.movementCost.plain.deadLeader = 4;
        settings.movementCost.plain.onRoad = 1;
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
        settings.unrestrictedBestowWards = true;
        settings.unitEncyclopedia.detailedUnitDescription = true;
        settings.unitEncyclopedia.detailedAttackDescription = true;
        settings.fixEffectiveHpFormula = true;

        // The default value of 1024 objects provides room for average object size of 512 bytes.
        settings.engine.sendRefreshInfoObjectCountLimit = 1024;

        initialized = true;
    }

    return settings;
}

void initializeUserSettings(Settings& value)
{
    value = defaultSettings();

    const auto path{scriptsFolder() / "settings.lua"};
    try {
        const auto env{executeScriptFile(path)};
        if (!env)
            return;

        const sol::table& table = (*env)["settings"];
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
