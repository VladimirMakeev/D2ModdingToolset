/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#include "attackutils.h"
#include "attack.h"
#include "attackmodified.h"
#include "customattacks.h"
#include "custommodifier.h"
#include "dynamiccast.h"
#include "globaldata.h"
#include "globalvariables.h"
#include "midgardid.h"
#include <array>

namespace hooks {

game::IAttack* getGlobalAttack(const game::CMidgardID* attackId)
{
    using namespace game;

    if (*attackId == emptyId)
        return nullptr;

    const auto& global = GlobalDataApi::get();
    const auto attacks = (*global.getGlobalData())->attacks;

    return (IAttack*)global.findById(attacks, attackId);
}

game::CAttackImpl* getAttackImpl(const game::IAttack* attack)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    auto current = attack;
    while (current) {
        auto customModifier = castAttackToCustomModifier(current);
        if (customModifier) {
            current = customModifier->getPrevAttack(current);
            continue;
        }

        auto attackImpl = (CAttackImpl*)dynamicCast(current, 0, rtti.IAttackType,
                                                    rtti.CAttackImplType, 0);
        if (attackImpl)
            return attackImpl;

        auto attackModified = (CAttackModified*)dynamicCast(current, 0, rtti.IAttackType,
                                                            rtti.CAttackModifiedType, 0);
        current = attackModified ? attackModified->data->prev : nullptr;
    }

    return nullptr;
}

int getBoostDamage(int level)
{
    using namespace game;

    const auto& global = GlobalDataApi::get();
    const auto vars = *(*global.getGlobalData())->globalVariables;

    int count = std::size(vars->battleBoostDamage);
    return (0 < level && level <= count) ? vars->battleBoostDamage[level - 1] : 0;
}

int getLowerDamage(int level)
{
    using namespace game;

    const auto& global = GlobalDataApi::get();
    const auto vars = *(*global.getGlobalData())->globalVariables;

    int count = std::size(vars->battleLowerDamage);
    return (0 < level && level <= count) ? vars->battleLowerDamage[level - 1] : 0;
}

int getLowerInitiative(int level)
{
    using namespace game;

    if (level != 1)
        return 0;

    const auto& global = GlobalDataApi::get();
    const auto vars = *(*global.getGlobalData())->globalVariables;

    return vars->battleLowerIni;
}

bool attackHasPower(game::AttackClassId id)
{
    const auto& classes = game::AttackClassCategories::get();

    return id == classes.paralyze->id || id == classes.petrify->id || id == classes.damage->id
           || id == classes.drain->id || id == classes.drainOverflow->id || id == classes.fear->id
           || id == classes.lowerDamage->id || id == classes.lowerInitiative->id
           || id == classes.poison->id || id == classes.frostbite->id || id == classes.blister->id
           || id == classes.bestowWards->id || id == classes.shatter->id || id == classes.revive->id
           || id == classes.drainLevel->id || id == classes.transformSelf->id
           || id == classes.transformOther->id;
}

bool attackHasDamage(game::AttackClassId id)
{
    const auto& classes = game::AttackClassCategories::get();

    return id == classes.damage->id || id == classes.drain->id || id == classes.drainOverflow->id
           || id == classes.poison->id || id == classes.frostbite->id || id == classes.blister->id
           || id == classes.shatter->id;
}

bool attackHasHeal(game::AttackClassId id)
{
    const auto& classes = game::AttackClassCategories::get();

    return id == classes.heal->id || id == classes.revive->id || id == classes.bestowWards->id;
}

bool attackHasInfinite(game::AttackClassId id)
{
    const auto& classes = game::AttackClassCategories::get();

    return id == classes.paralyze->id || id == classes.petrify->id || id == classes.boostDamage->id
           || id == classes.lowerDamage->id || id == classes.lowerInitiative->id
           || id == classes.poison->id || id == classes.frostbite->id || id == classes.blister->id
           || id == classes.transformOther->id;
}

bool attackHasCritHit(game::AttackClassId id)
{
    return isNormalDamageAttack(id);
}

bool attackHasAltAttack(game::AttackClassId id)
{
    const auto& classes = game::AttackClassCategories::get();

    return id == classes.transformSelf->id || id == classes.doppelganger->id;
}

bool attackHasAltAttack(const game::IAttack* attack)
{
    // Using getAttackClass here results in a serious performance loss due to recursive calls inside
    // of CCustomModifier::attackGetAttackClass in case of nested modifiers
    const auto altAttackId = attack->vftable->getAltAttackId(attack);
    return *altAttackId != game::emptyId;
}

bool attackHasDrain(game::AttackClassId id)
{
    return isNormalDamageAttack(id);
}

bool isNormalDamageAttack(game::AttackClassId id)
{
    const auto& classes = game::AttackClassCategories::get();

    return id == classes.damage->id || id == classes.drain->id || id == classes.drainOverflow->id;
}

bool isModifiableDamageAttack(game::AttackClassId id)
{
    const auto& classes = game::AttackClassCategories::get();

    return id == classes.damage->id || id == classes.drain->id || id == classes.drainOverflow->id
           || id == classes.shatter->id;
}

bool isMeleeAttack(const game::IAttack* attack)
{
    auto reach = attack->vftable->getAttackReach(attack);
    return isMeleeAttack(reach->id);
}

bool isMeleeAttack(game::AttackReachId id)
{
    using namespace game;

    const auto& reaches = AttackReachCategories::get();

    if (id == reaches.adjacent->id) {
        return true;
    } else if (id != reaches.all->id && id != reaches.any->id) {
        for (auto& custom : getCustomAttacks().reaches) {
            if (id == custom.reach.id) {
                return custom.melee;
            }
        }
    }

    return false;
}

int getAttackMaxTargets(const game::AttackReachId id)
{
    using namespace game;

    const auto& reaches = AttackReachCategories::get();

    if (id == reaches.all->id) {
        return 6;
    } else if (id == reaches.any->id || id == reaches.adjacent->id) {
        return 1;
    } else {
        for (const auto& custom : getCustomAttacks().reaches) {
            if (id == custom.reach.id) {
                return custom.maxTargets;
            }
        }
    }

    return 0;
}

const game::LAttackSource* getAttackSourceById(game::AttackSourceId id)
{
    using namespace game;

    const auto& attackSources{AttackSourceCategories::get()};

    switch (id) {
    default: {
        const auto& customSources = hooks::getCustomAttacks().sources;
        for (const auto& customSource : customSources) {
            if (customSource.source.id == id) {
                return &customSource.source;
            }
        }

        // Could not find source id even in custom sources
        return nullptr;
    }
    case AttackSourceId::Weapon:
        return attackSources.weapon;

    case AttackSourceId::Mind:
        return attackSources.mind;

    case AttackSourceId::Life:
        return attackSources.life;

    case AttackSourceId::Death:
        return attackSources.death;

    case AttackSourceId::Fire:
        return attackSources.fire;

    case AttackSourceId::Water:
        return attackSources.water;

    case AttackSourceId::Earth:
        return attackSources.earth;

    case AttackSourceId::Air:
        return attackSources.air;
    }
}

const game::LAttackClass* getAttackClassById(game::AttackClassId id)
{
    using namespace game;

    const auto& attackClasses{AttackClassCategories::get()};

    switch (id) {
    case AttackClassId::Damage:
        return attackClasses.damage;

    case AttackClassId::Drain:
        return attackClasses.drain;

    case AttackClassId::Paralyze:
        return attackClasses.paralyze;

    case AttackClassId::Heal:
        return attackClasses.heal;

    case AttackClassId::Fear:
        return attackClasses.fear;

    case AttackClassId::BoostDamage:
        return attackClasses.boostDamage;

    case AttackClassId::Petrify:
        return attackClasses.petrify;

    case AttackClassId::LowerDamage:
        return attackClasses.lowerDamage;

    case AttackClassId::LowerInitiative:
        return attackClasses.lowerInitiative;

    case AttackClassId::Poison:
        return attackClasses.poison;

    case AttackClassId::Frostbite:
        return attackClasses.frostbite;

    case AttackClassId::Revive:
        return attackClasses.revive;

    case AttackClassId::DrainOverflow:
        return attackClasses.drainOverflow;

    case AttackClassId::Cure:
        return attackClasses.cure;

    case AttackClassId::Summon:
        return attackClasses.summon;

    case AttackClassId::DrainLevel:
        return attackClasses.drainLevel;

    case AttackClassId::GiveAttack:
        return attackClasses.giveAttack;

    case AttackClassId::Doppelganger:
        return attackClasses.doppelganger;

    case AttackClassId::TransformSelf:
        return attackClasses.transformSelf;

    case AttackClassId::TransformOther:
        return attackClasses.transformOther;

    case AttackClassId::Blister:
        return attackClasses.blister;

    case AttackClassId::BestowWards:
        return attackClasses.bestowWards;

    case AttackClassId::Shatter:
        return attackClasses.shatter;
    }

    return nullptr;
}

} // namespace hooks
