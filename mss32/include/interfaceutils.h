/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#ifndef INTERFACEUTILS_H
#define INTERFACEUTILS_H

#include <string>

namespace game {
struct LAttackSource;
struct IEncUnitDescriptor;
struct CMidUnitDescriptor;
struct CUnitTypeDescriptor;
struct CLeaderUnitDescriptor;
struct CDynUpgrade;
struct TUsUnitImpl;

enum class AttackSourceId : int;
enum class AttackClassId : int;
} // namespace game

namespace hooks {

const game::CMidUnitDescriptor* castToMidUnitDescriptor(const game::IEncUnitDescriptor* descriptor);
const game::CUnitTypeDescriptor* castToUnitTypeDescriptor(
    const game::IEncUnitDescriptor* descriptor);
const game::CLeaderUnitDescriptor* castToLeaderUnitDescriptor(
    const game::IEncUnitDescriptor* descriptor);
const game::TUsUnitImpl* getUnitImpl(const game::IEncUnitDescriptor* descriptor);
bool hasCriticalHitLeaderAbility(const game::IEncUnitDescriptor* descriptor);

std::string getNumberText(int value, bool percent);
std::string getBonusNumberText(int bonus, bool percent, bool reverse);
std::string addBonusNumberText(const std::string& base, int bonus, bool percent, bool reverse);
std::string getModifiedNumberText(int value, int base, bool percent);
std::string getModifiedNumberTextFull(int value, int base, bool percent);
std::string getModifiedNumberTextReverseBonus(int value, int base, bool percent);
std::string getModifiedNumberTextTotal(int value, int base, bool percent);
std::string getModifiedNumberTextTotalReverseBonus(int value, int base, bool percent);
std::string getModifiedStringText(const std::string& value, bool modified);

std::string getAttackSourceText(const game::LAttackSource* source);
std::string getAttackSourceText(game::AttackSourceId id);
std::string getAttackClassText(game::AttackClassId id);

bool getDynUpgradesToDisplay(game::IEncUnitDescriptor* descriptor,
                             const game::CDynUpgrade** upgrade1,
                             const game::CDynUpgrade** upgrade2);
void addDynUpgradeLevelToField(std::string& text, const char* field, int level);
void addDynUpgradeTextToField(std::string& text, const char* field, int upgrade1, int upgrade2);

} // namespace hooks

#endif // INTERFACEUTILS_H
