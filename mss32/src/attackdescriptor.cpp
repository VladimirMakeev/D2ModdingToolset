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
#include "midunit.h"
#include "midunitdescriptor.h"
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
        auto altAttackId = attack->vftable->getAltAttackId(attack);
        return *altAttackId != emptyId ? attack : nullptr;
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

    auto attack = hooks::getAttack(&attackId);
    if (attack == nullptr && type == AttackType::Primary) {
        hooks::generateUnitImplByAttackId(&attackId);
        attack = hooks::getAttack(&attackId);
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

    static LAttackClass emptyClass = {nullptr, nullptr, (AttackClassId)emptyCategoryId};
    static LAttackSource emptySource = {nullptr, nullptr, (AttackSourceId)emptyCategoryId};
    static LAttackReach emptyReach = {nullptr, nullptr, (AttackReachId)emptyCategoryId};

    data = {};
    if (attack == nullptr) {
        data.empty = true;
        data.class_ = &emptyClass;
        data.source = &emptySource;
        data.reach = &emptyReach;
    } else if (descriptor && type == AttackType::Primary) {
        data.name = descriptor->vftable->getAttackName(descriptor);
        data.class_ = descriptor->vftable->getAttackClass(descriptor);
        data.source = descriptor->vftable->getAttackSource(descriptor);
        data.reach = descriptor->vftable->getAttackReach(descriptor);
        data.damage = descriptor->vftable->getAttackDamageOrHeal(descriptor);
        data.heal = data.damage;
        data.power = descriptor->vftable->getAttackPower(descriptor);
        data.initiative = descriptor->vftable->getAttackInitiative(descriptor);
        data.level = descriptor->vftable->getAttackLevel(descriptor);
        data.infinite = attack->vftable->getInfinite(attack);
        data.critHit = attack->vftable->getCritHit(attack);
        data.custom = hooks::getCustomAttackData(attack);
    } else {
        data.class_ = attack->vftable->getAttackClass(attack);
        data.source = attack->vftable->getAttackSource(attack);
        data.reach = attack->vftable->getAttackReach(attack);
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

const game::LAttackClass* AttackDescriptor::class_() const
{
    return data.class_;
}

const game::LAttackSource* AttackDescriptor::source() const
{
    return data.source;
}

const game::LAttackReach* AttackDescriptor::reach() const
{
    return data.reach;
}

int AttackDescriptor::damage() const
{
    return data.damage;
}

int AttackDescriptor::heal() const
{
    return data.heal;
}

int AttackDescriptor::power() const
{
    return hooks::attackHasPower(data.class_) ? data.power : 100;
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
    return data.critHit ? custom().critDamage : 0;
}

std::uint8_t AttackDescriptor::critPower() const
{
    return data.critHit ? custom().critPower : 0;
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

int AttackDescriptor::maxTargets() const
{
    using namespace game;

    const auto& reaches = AttackReachCategories::get();

    int maxTargets = 0;
    auto reachId = data.reach->id;
    if (reachId == reaches.all->id) {
        maxTargets = 6;
    } else if (reachId == reaches.any->id || reachId == reaches.adjacent->id) {
        maxTargets = 1;
    } else {
        for (const auto& custom : hooks::getCustomAttacks().reaches) {
            if (reachId == custom.reach.id) {
                maxTargets = custom.maxTargets;
                break;
            }
        }
    }

    return maxTargets;
}

const hooks::CustomAttackData& AttackDescriptor::custom() const
{
    return data.custom;
}

} // namespace utils
