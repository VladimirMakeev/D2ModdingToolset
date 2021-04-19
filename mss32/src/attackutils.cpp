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

namespace hooks {

bool isAttackClassUsesAccuracy(const game::LAttackClass* attackClass)
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
