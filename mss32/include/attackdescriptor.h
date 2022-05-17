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

#ifndef ATTACKDESCRIPTOR_H
#define ATTACKDESCRIPTOR_H

#include "customattacks.h"
#include "idlist.h"

namespace game {
struct IEncUnitDescriptor;
struct IAttack;
struct LAttackClass;
struct LAttackSource;
struct LAttackReach;
} // namespace game

namespace utils {

/**
 * Note that in terms of descriptions 'Alternative' attack means Doppelganger / TransformSelf.
 * Despite that in internal game structure (like Gattacks / IUsSoldier) those attack are primary,
 * having normal attack as alternative.
 */
enum class AttackType : int
{
    Primary,
    Secondary,
    Alternative,
};

struct AttackDescriptorData
{
    bool empty;
    std::string name;
    const game::LAttackClass* class_;
    const game::LAttackSource* source;
    const game::LAttackReach* reach;
    int damage;
    int heal;
    int power;
    int initiative;
    int level;
    bool infinite;
    bool critHit;
    hooks::CustomAttackData custom;
};

class AttackDescriptor
{
public:
    AttackDescriptor(game::IEncUnitDescriptor* descriptor, AttackType type, bool global);

    bool empty() const;
    std::string name() const;
    const game::LAttackClass* class_() const;
    const game::LAttackSource* source() const;
    const game::LAttackReach* reach() const;
    int damage(const game::IdList* modifiers = nullptr) const;
    int heal() const;
    bool hasPower() const;
    int power(const game::IdList* modifiers = nullptr) const;
    int initiative(const game::IdList* modifiers = nullptr) const;
    int level() const;
    int boost() const;
    int lower() const;
    int lowerIni() const;
    bool infinite() const;
    bool critHit() const;
    std::uint8_t critDamage() const;
    std::uint8_t critPower() const;
    std::uint8_t damageRatio() const;
    bool damageRatioPerTarget() const;
    bool damageSplit() const;
    const hooks::CustomAttackData& custom() const;

private:
    void initialize(game::IAttack* attack, game::IEncUnitDescriptor* descriptor, AttackType type);

    AttackDescriptorData data;
};

} // namespace utils

#endif // ATTACKDESCRIPTOR_H
