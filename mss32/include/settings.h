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
#include <string>

namespace hooks {

struct Color
{
    std::uint8_t r{};
    std::uint8_t g{};
    std::uint8_t b{};
};

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
    std::uint8_t criticalHitChance;
    std::uint8_t mageLeaderAttackPowerReduction;
    std::uint8_t disableAllowedRoundMax;
    std::uint8_t shatterDamageUpgradeRatio;

    struct AiAttackPowerBonus
    {
        std::int8_t easy;
        std::int8_t average;
        std::int8_t hard;
        std::int8_t veryHard;
        bool absolute;
    } aiAttackPowerBonus;

    bool showBanners;
    bool showResources;
    bool showLandConverted;
    bool preserveCapitalBuildings;
    bool allowShatterAttackToMiss;
    bool doppelgangerRespectsEnemyImmunity;
    bool doppelgangerRespectsAllyImmunity;
    bool leveledDoppelgangerAttack;
    bool leveledTransformSelfAttack;
    bool leveledSummonAttack;
    bool missChanceSingleRoll;
    bool unrestrictedBestowWards;
    bool freeTransformSelfAttack;
    bool detailedAttackDescription;

    struct MovementCost
    {
        Color textColor{};
        Color outlineColor{};
        bool show{};
    } movementCost;

    struct Lobby
    {
        struct Server
        {
            std::string ip{"127.0.0.1"};
            std::uint16_t port{61111};
        } server;

        struct Client
        {
            std::uint16_t port{};
        } client;
    } lobby;

    bool debugMode;
};

const Settings& baseSettings();
const Settings& defaultSettings();
const Settings& userSettings();

} // namespace hooks

#endif // SETTINGS_H
