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
#include "dynamiccast.h"
#include "game.h"
#include "midunit.h"
#include "midunitdescriptor.h"
#include "ummodifier.h"
#include "unitutils.h"

namespace utils {

game::IAttack* getAttack(game::IUsSoldier* soldier, AttackType type)
{
    using namespace game;

    switch (type) {
    case AttackType::Primary:
        return hooks::getAttack(soldier, true, true);
    case AttackType::Secondary:
        return hooks::getAttack(soldier, false, false);
    case AttackType::Alternative: {
        auto attack = hooks::getAttack(soldier, true, false);
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

AttackDescriptor::AttackDescriptor(game::IEncUnitDescriptor* descriptor,
                                   AttackType type,
                                   bool global)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    if (global) {
        initialize(getGlobalAttack(descriptor, type), nullptr, type);
    } else {
        auto midUnitDescriptor = (const game::CMidUnitDescriptor*)
            dynamicCast(descriptor, 0, rtti.IEncUnitDescriptorType, rtti.CMidUnitDescriptorType, 0);
        if (midUnitDescriptor) {
            const auto unitImpl = midUnitDescriptor->unit->unitImpl;
            const auto soldier = hooks::castUnitImplToSoldierWithLogging(unitImpl);
            initialize(getAttack(soldier, type), nullptr, type);
        } else {
            initialize(getGlobalAttack(descriptor, type), descriptor, type);
        }
    }
}

void AttackDescriptor::initialize(game::IAttack* attack,
                                  game::IEncUnitDescriptor* descriptor,
                                  AttackType type)
{
    using namespace game;

    data = {};
    if (attack == nullptr) {
        data.empty = true;
        data.classId = (AttackClassId)emptyCategoryId;
        data.sourceId = (AttackSourceId)emptyCategoryId;
        data.reachId = (AttackReachId)emptyCategoryId;
    } else if (descriptor && type == AttackType::Primary) {
        data.name = descriptor->vftable->getAttackName(descriptor);
        data.classId = descriptor->vftable->getAttackClass(descriptor)->id;
        data.sourceId = descriptor->vftable->getAttackSource(descriptor)->id;
        data.reachId = descriptor->vftable->getAttackReach(descriptor)->id;
        data.damage = descriptor->vftable->getAttackDamageOrHeal(descriptor);
        data.heal = data.damage;
        data.power = descriptor->vftable->getAttackPower(descriptor);
        data.initiative = descriptor->vftable->getAttackInitiative(descriptor);
        data.level = descriptor->vftable->getAttackLevel(descriptor);
        data.infinite = attack->vftable->getInfinite(attack);
        data.critHit = attack->vftable->getCritHit(attack);
        data.custom = hooks::getCustomAttackData(attack);
    } else {
        data.classId = attack->vftable->getAttackClass(attack)->id;
        data.sourceId = attack->vftable->getAttackSource(attack)->id;
        data.reachId = attack->vftable->getAttackReach(attack)->id;
        data.damage = attack->vftable->getQtyDamage(attack);
        data.heal = attack->vftable->getQtyHeal(attack);
        data.initiative = attack->vftable->getInitiative(attack);
        data.level = attack->vftable->getLevel(attack);
        data.infinite = attack->vftable->getInfinite(attack);
        data.critHit = attack->vftable->getCritHit(attack);
        data.custom = hooks::getCustomAttackData(attack);

        if (descriptor && type == AttackType::Secondary) {
            data.name = descriptor->vftable->getAttack2Name(descriptor);
            data.power = descriptor->vftable->getAttack2Power(descriptor);
        } else if (descriptor && type == AttackType::Alternative) {
            data.name = descriptor->vftable->getAltAttackName(descriptor);
            data.power = descriptor->vftable->getAltAttackPower(descriptor);
        } else {
            int tmp;
            data.name = attack->vftable->getName(attack);
            data.power = *attack->vftable->getPower(attack, &tmp);
        }
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

int AttackDescriptor::damage(const game::IdList* modifiers) const
{
    using namespace game;

    const auto& fn = gameFunctions();

    if (!hooks::attackHasDamage(data.classId))
        return 0;

    if (!modifiers)
        return data.damage;

    return fn.applyPercentModifiers(data.damage, modifiers, ModifierElementTypeFlag::QtyDamage);
}

int AttackDescriptor::heal() const
{
    return data.heal;
}

bool AttackDescriptor::hasPower() const
{
    return hooks::attackHasPower(data.classId);
}

int AttackDescriptor::power(const game::IdList* modifiers) const
{
    using namespace game;

    const auto& fn = gameFunctions();

    if (!hasPower())
        return 100;

    if (!modifiers)
        return data.power;

    return fn.applyPercentModifiers(data.power, modifiers, ModifierElementTypeFlag::Power);
}

int AttackDescriptor::initiative(const game::IdList* modifiers) const
{
    using namespace game;

    const auto& fn = gameFunctions();

    if (!modifiers)
        return data.initiative;

    return fn.applyPercentModifiers(data.initiative, modifiers,
                                    ModifierElementTypeFlag::Initiative);
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
    return hooks::attackHasInfinite(data.classId) ? data.infinite : false;
}

bool AttackDescriptor::critHit() const
{
    return hooks::attackHasCritHit(data.classId) ? data.critHit : false;
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
