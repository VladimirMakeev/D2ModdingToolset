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
    std::uint8_t splitDamageMultiplier;

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
    bool buildTempleForWarriorLord;
    bool allowShatterAttackToMiss;
    bool doppelgangerRespectsEnemyImmunity;
    bool doppelgangerRespectsAllyImmunity;
    bool leveledDoppelgangerAttack;
    bool leveledTransformSelfAttack;
    bool leveledTransformOtherAttack;
    bool leveledDrainLevelAttack;
    bool leveledSummonAttack;
    bool missChanceSingleRoll;
    bool unrestrictedBestowWards;
    bool freeTransformSelfAttack;
    bool freeTransformSelfAttackInfinite;
    bool fixEffectiveHpFormula;

    struct UnitEncyclopedia
    {
        bool detailedUnitDescription;
        bool detailedAttackDescription;
        bool displayDynamicUpgradeValues;
        bool displayBonusHp;
        bool displayBonusXp;
        bool displayInfiniteAttackIndicator;
        bool displayCriticalHitTextInAttackName;
    } unitEncyclopedia;

    struct Modifiers
    {
        bool cumulativeUnitRegeneration;
        bool notifyModifiersChanged;
        bool validateUnitsOnGroupChanged;
    } modifiers;

    struct AllowBattleItems
    {
        bool onTransformOther;
        bool onTransformSelf;
        bool onDrainLevel;
        bool onDoppelganger;
    } allowBattleItems;

    struct MovementCost
    {
        struct Water
        {
            int dflt;
            int deadLeader;
            int withBonus;
            int waterOnly;
        } water;

        struct Forest
        {
            int dflt;
            int deadLeader;
            int withBonus;
        } forest;

        struct Plain
        {
            int dflt;
            int deadLeader;
            int onRoad;
        } plain;

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
            std::uint16_t port{1};
        } client;
    } lobby;

    // Do not expose these settings in public 'settings.lua' template so poor souls won't suffer
    // from their own ignorance
    struct Debug
    {
        std::uint32_t sendObjectsChangesTreshold{0};
        bool logSinglePlayerMessages{false};
    } debug;

    struct Engine
    {
        // This is needed to split single CRefreshInfo into several instances when loading large
        // scenario, because it needs to fit to the network message buffer of 512 KB.
        std::uint32_t sendRefreshInfoObjectCountLimit{0};
    } engine;

    struct Battle
    {
        bool allowRetreatedUnitsToUpgrade{false};
        bool carryXpOverUpgrade{false};
        bool allowMultiUpgrade{false};
    } battle;

    bool debugMode;
};

const Settings& baseSettings();
const Settings& defaultSettings();
const Settings& userSettings();

} // namespace hooks

#endif // SETTINGS_H
