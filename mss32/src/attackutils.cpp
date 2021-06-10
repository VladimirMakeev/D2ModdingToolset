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
#include "dynamiccast.h"
#include "globaldata.h"
#include "globalvariables.h"
#include "midgardid.h"

namespace hooks {

const game::Restriction<int> attackPowerLimits{-100, 100};
const game::Restriction<int> attackInitiativeLimits{1, 150};

game::IAttack* getAttack(const game::CMidgardID* attackId)
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
        auto attackImpl = (CAttackImpl*)dynamicCast(current, 0, rtti.IAttackType,
                                                    rtti.CAttackImplType, 0);
        if (attackImpl)
            return attackImpl;

        auto attackModified = (CAttackModified*)dynamicCast(current, 0, rtti.IAttackType,
                                                            rtti.CAttackModifiedType, 0);
        current = attackModified ? attackModified->data->impl : nullptr;
    }

    return nullptr;
}

int getBoostDamage(int level)
{
    using namespace game;

    const auto& global = GlobalDataApi::get();
    const auto vars = *(*global.getGlobalData())->globalVariables;

    int count = sizeof(vars->battleBoostDamage) / sizeof(*vars->battleBoostDamage);
    return (0 < level && level <= count) ? vars->battleBoostDamage[level - 1] : 0;
}

int getLowerDamage(int level)
{
    using namespace game;

    const auto& global = GlobalDataApi::get();
    const auto vars = *(*global.getGlobalData())->globalVariables;

    int count = sizeof(vars->battleLowerDamage) / sizeof(*vars->battleLowerDamage);
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
    const auto& attacks = game::AttackClassCategories::get();
    const auto id = attackClass->id;

    return id == attacks.paralyze->id || id == attacks.petrify->id || id == attacks.damage->id
           || id == attacks.drain->id || id == attacks.drainOverflow->id || id == attacks.fear->id
           || id == attacks.lowerDamage->id || id == attacks.lowerInitiative->id
           || id == attacks.poison->id || id == attacks.frostbite->id || id == attacks.blister->id
           || id == attacks.bestowWards->id || id == attacks.shatter->id || id == attacks.revive->id
           || id == attacks.drainLevel->id || id == attacks.transformSelf->id
           || id == attacks.transformOther->id;
}

} // namespace hooks
