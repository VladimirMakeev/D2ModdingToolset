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

bool attackHasPower(const game::LAttackClass* attackClass)
{
    const auto& classes = game::AttackClassCategories::get();
    const auto id = attackClass->id;

    return id == classes.paralyze->id || id == classes.petrify->id || id == classes.damage->id
           || id == classes.drain->id || id == classes.drainOverflow->id || id == classes.fear->id
           || id == classes.lowerDamage->id || id == classes.lowerInitiative->id
           || id == classes.poison->id || id == classes.frostbite->id || id == classes.blister->id
           || id == classes.bestowWards->id || id == classes.shatter->id || id == classes.revive->id
           || id == classes.drainLevel->id || id == classes.transformSelf->id
           || id == classes.transformOther->id;
}

bool isMeleeAttack(const game::IAttack* attack)
{
    using namespace game;

    const auto& reaches = AttackReachCategories::get();

    auto reach = attack->vftable->getAttackReach(attack);
    if (reach->id == reaches.adjacent->id) {
        return true;
    } else if (reach->id != reaches.all->id && reach->id != reaches.any->id) {
        for (auto& custom : getCustomAttacks().reaches) {
            if (reach->id == custom.reach.id) {
                return custom.melee;
            }
        }
    }

    return false;
}

int getAttackMaxTargets(const game::LAttackReach* reach)
{
    using namespace game;

    const auto& reaches = AttackReachCategories::get();

    if (reach->id == reaches.all->id) {
        return 6;
    } else if (reach->id == reaches.any->id || reach->id == reaches.adjacent->id) {
        return 1;
    } else {
        for (const auto& custom : getCustomAttacks().reaches) {
            if (reach->id == custom.reach.id) {
                return custom.maxTargets;
            }
        }
    }

    return 0;
}

} // namespace hooks
