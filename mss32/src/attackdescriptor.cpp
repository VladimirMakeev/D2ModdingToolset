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

#include "attackdescriptor.h"
#include "attack.h"
#include "attackutils.h"
#include "customattacks.h"
#include "customattackutils.h"
#include "game.h"
#include "interfaceutils.h"
#include "midunit.h"
#include "midunitdescriptor.h"
#include "restrictions.h"
#include "settings.h"
#include "ummodifier.h"
#include "unitutils.h"
#include "usunitimpl.h"

namespace utils {

game::IAttack* getAttack(game::IUsUnit* unitImpl, AttackType type)
{
    using namespace game;

    switch (type) {
    case AttackType::Primary:
        return hooks::getAttack(unitImpl, true, true);
    case AttackType::Secondary:
        return hooks::getAttack(unitImpl, false, false);
    case AttackType::Alternative: {
        auto attack = hooks::getAttack(unitImpl, true, false);
        if (*attack->vftable->getAltAttackId(attack) != emptyId)
            return attack;
    }
    }

    return nullptr;
}

game::IAttack* getGlobalAttack(game::IEncUnitDescriptor* descriptor, AttackType type)
{
    using namespace game;

    CMidgardID attackId{};
    switch (type) {
    case AttackType::Primary:
        descriptor->vftable->getAttackId(descriptor, &attackId);
        break;
    case AttackType::Secondary:
        descriptor->vftable->getAttack2Id(descriptor, &attackId);
        break;
    case AttackType::Alternative:
        descriptor->vftable->getAltAttackId(descriptor, &attackId);
        break;
    }

    if (attackId == emptyId)
        return nullptr;

    auto attack = hooks::getGlobalAttack(&attackId);
    if (attack == nullptr && type == AttackType::Primary) {
        hooks::generateUnitImplByAttackId(&attackId);
        attack = hooks::getGlobalAttack(&attackId);
    }

    return attack;
}

game::IAttack* getAttack(game::IEncUnitDescriptor* descriptor,
                         AttackType type,
                         bool global,
                         bool* useDescriptor)
{
    *useDescriptor = false;

    if (global) {
        return getGlobalAttack(descriptor, type);
    }

    auto midUnitDescriptor = hooks::castToMidUnitDescriptor(descriptor);
    if (midUnitDescriptor) {
        return getAttack(midUnitDescriptor->unit->unitImpl, type);
    } else {
        *useDescriptor = true;
        return getGlobalAttack(descriptor, type);
    }
}

int computeValue(int base,
                 int min,
                 int max,
                 int boost,
                 const game::IdList* modifiers,
                 game::ModifierElementTypeFlag modifierType)
{
    using namespace game;

    int result = base;

    if (modifiers) {
        result = gameFunctions().applyPercentModifiers(result, modifiers, modifierType);
    }

    if (boost) {
        result += result * boost / 100;
    }

    return std::clamp(result, min, max);
}

int computeDamage(int base,
                  int boostDamageLevel,
                  int lowerDamageLevel,
                  const game::IdList* modifiers,
                  int damageMax,
                  game::AttackClassId classId,
                  bool damageSplit)
{
    using namespace game;

    const auto& restrictions = gameRestrictions();

    int result = base;
    if (hooks::isNormalDamageAttack(classId)) {
        int boost = hooks::getBoostDamage(boostDamageLevel)
                    - hooks::getLowerDamage(lowerDamageLevel);
        result = computeValue(base, restrictions.unitDamage->min, damageMax, boost, modifiers,
                              ModifierElementTypeFlag::QtyDamage);

        if (damageSplit) {
            result *= hooks::userSettings().splitDamageMultiplier;
        }
    } else if (hooks::isModifiableDamageAttack(classId)) {
        result = computeValue(base, restrictions.unitDamage->min, damageMax, 0, modifiers,
                              ModifierElementTypeFlag::QtyDamage);
    }

    if (classId == AttackClassId::Shatter) {
        if (result > hooks::userSettings().shatterDamageMax) {
            result = hooks::userSettings().shatterDamageMax;
        }
    }

    return result;
}

int computePower(int base, const game::IdList* modifiers, game::AttackClassId classId)
{
    using namespace game;

    const auto& restrictions = gameRestrictions();

    if (!hooks::attackHasPower(classId))
        return 100;

    return computeValue(base, restrictions.attackPower->min, restrictions.attackPower->max, 0,
                        modifiers, ModifierElementTypeFlag::Power);
}

int computeInitiative(int base, int lowerInitiativeLevel, const game::IdList* modifiers)
{
    using namespace game;

    const auto& restrictions = gameRestrictions();

    return computeValue(base, restrictions.attackInitiative->min,
                        restrictions.attackInitiative->max,
                        -hooks::getLowerInitiative(lowerInitiativeLevel), modifiers,
                        ModifierElementTypeFlag::Initiative);
}

AttackDescriptor::AttackDescriptor(game::IEncUnitDescriptor* descriptor,
                                   AttackType type,
                                   bool global,
                                   int boostDamageLevel,
                                   int lowerDamageLevel,
                                   int lowerInitiativeLevel,
                                   const game::IdList* modifiers,
                                   int damageMax)
    : data()
{
    using namespace hooks;

    bool useDescriptor;
    auto attack = getAttack(descriptor, type, global, &useDescriptor);
    if (attack == nullptr) {
        data.empty = true;
        data.classId = (game::AttackClassId)game::emptyCategoryId;
        data.sourceId = (game::AttackSourceId)game::emptyCategoryId;
        data.reachId = (game::AttackReachId)game::emptyCategoryId;
        return;
    }

    if (useDescriptor && type == AttackType::Primary) {
        data.name = descriptor->vftable->getAttackName(descriptor);
        data.classId = descriptor->vftable->getAttackClass(descriptor)->id;
        data.sourceId = descriptor->vftable->getAttackSource(descriptor)->id;
        data.reachId = descriptor->vftable->getAttackReach(descriptor)->id;
        data.initiative = descriptor->vftable->getAttackInitiative(descriptor);
        data.level = descriptor->vftable->getAttackLevel(descriptor);

        if (attackHasDamage(data.classId)) {
            data.damage = descriptor->vftable->getAttackDamageOrHeal(descriptor);
        } else if (attackHasHeal(data.classId)) {
            data.heal = descriptor->vftable->getAttackDamageOrHeal(descriptor);
        }

        if (attackHasPower(data.classId)) {
            data.power = descriptor->vftable->getAttackPower(descriptor);
        }
    } else {
        data.classId = attack->vftable->getAttackClass(attack)->id;
        data.sourceId = attack->vftable->getAttackSource(attack)->id;
        data.reachId = attack->vftable->getAttackReach(attack)->id;
        data.initiative = attack->vftable->getInitiative(attack);
        data.level = attack->vftable->getLevel(attack);

        if (attackHasDamage(data.classId)) {
            data.damage = attack->vftable->getQtyDamage(attack);
        } else if (attackHasHeal(data.classId)) {
            data.heal = attack->vftable->getQtyHeal(attack);
        }

        if (useDescriptor && type == AttackType::Secondary) {
            data.name = descriptor->vftable->getAttack2Name(descriptor);
            if (attackHasPower(data.classId)) {
                data.power = descriptor->vftable->getAttack2Power(descriptor);
            }
        } else if (useDescriptor && type == AttackType::Alternative) {
            data.name = descriptor->vftable->getAltAttackName(descriptor);
            if (attackHasPower(data.classId)) {
                data.power = descriptor->vftable->getAltAttackPower(descriptor);
            }
        } else {
            data.name = attack->vftable->getName(attack);
            if (attackHasPower(data.classId)) {
                int tmp;
                data.power = *attack->vftable->getPower(attack, &tmp);
            }
        }
    }

    data.custom = getCustomAttackData(attack);
    data.initiative = computeInitiative(data.initiative, lowerInitiativeLevel, modifiers);
    data.damage = computeDamage(data.damage, boostDamageLevel, lowerDamageLevel, modifiers,
                                damageMax, data.classId, data.custom.damageSplit);
    data.drain = attackHasDrain(data.classId) ? attack->vftable->getDrain(attack, data.damage) : 0;
    data.power = computePower(data.power, modifiers, data.classId);
    data.infinite = attackHasInfinite(data.classId) ? attack->vftable->getInfinite(attack) : 0;

    data.critHit = false;
    if (attackHasCritHit(data.classId)) {
        data.critHit = attack->vftable->getCritHit(attack);
        if (!data.critHit) {
            if (global) {
                auto unitImpl = getUnitImpl(descriptor);
                data.critHit = hasCriticalHitLeaderAbility(unitImpl);
            } else {
                data.critHit = hasCriticalHitLeaderAbility(descriptor);
            }
        }
    }

    switch (data.classId) {
    case game::AttackClassId::Drain:
        data.drain += data.damage * userSettings().drainAttackHeal / 100;
        break;
    case game::AttackClassId::DrainOverflow:
        data.drain += data.damage * userSettings().drainOverflowHeal / 100;
        break;
    }
}

bool AttackDescriptor::empty() const
{
    return data.empty;
}

std::string AttackDescriptor::name() const
{
    return data.name;
}

game::AttackClassId AttackDescriptor::classId() const
{
    return data.classId;
}

game::AttackSourceId AttackDescriptor::sourceId() const
{
    return data.sourceId;
}

game::AttackReachId AttackDescriptor::reachId() const
{
    return data.reachId;
}

int AttackDescriptor::damage() const
{
    return data.damage;
}

int AttackDescriptor::drain() const
{
    return data.drain;
}

int AttackDescriptor::heal() const
{
    return data.heal;
}

bool AttackDescriptor::hasPower() const
{
    return hooks::attackHasPower(data.classId);
}

int AttackDescriptor::power() const
{
    return data.power;
}

int AttackDescriptor::initiative() const
{
    return data.initiative;
}

int AttackDescriptor::level() const
{
    return data.level;
}

int AttackDescriptor::boost() const
{
    return hooks::getBoostDamage(data.level);
}

int AttackDescriptor::lower() const
{
    return hooks::getLowerDamage(data.level);
}

int AttackDescriptor::lowerIni() const
{
    return hooks::getLowerInitiative(data.level);
}

bool AttackDescriptor::infinite() const
{
    return data.infinite;
}

bool AttackDescriptor::critHit() const
{
    return data.critHit;
}

std::uint8_t AttackDescriptor::critDamage() const
{
    return critHit() ? custom().critDamage : 0;
}

std::uint8_t AttackDescriptor::critPower() const
{
    return critHit() ? custom().critPower : 0;
}

std::uint8_t AttackDescriptor::damageRatio() const
{
    return custom().damageRatio;
}

bool AttackDescriptor::damageRatioPerTarget() const
{
    return custom().damageRatioPerTarget;
}

bool AttackDescriptor::damageSplit() const
{
    return custom().damageSplit;
}

const hooks::CustomAttackData& AttackDescriptor::custom() const
{
    return data.custom;
}

} // namespace utils
