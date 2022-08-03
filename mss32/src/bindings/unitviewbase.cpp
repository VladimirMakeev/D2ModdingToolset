/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#include "unitviewbase.h"
#include "modifierview.h"
#include "unitimplview.h"
#include <sol/sol.hpp>

namespace bindings {

std::optional<UnitImplView> UnitViewBase::getImpl() const
{
    return {getImplInternal()};
}

std::optional<UnitImplView> UnitViewBase::getBaseImpl() const
{
    return getImpl()->getGlobal();
}

std::optional<UnitImplView> UnitViewBase::getLeveledImpl() const
{
    return getImpl()->getGenerated();
}

IdView UnitViewBase::getId() const
{
    return IdView{getIdInternal()};
}

int UnitViewBase::getXp() const
{
    return getXpInternal();
}

int UnitViewBase::getHp() const
{
    return getHpInternal();
}

int UnitViewBase::getHpMax() const
{
    return getHpMaxInternal();
}

int UnitViewBase::getLeaderCategory() const
{
    return getImpl()->getLeaderCategory();
}

int UnitViewBase::getMovement() const
{
    return getImpl()->getMovement();
}

int UnitViewBase::getScout() const
{
    return getImpl()->getScout();
}

int UnitViewBase::getLeadership() const
{
    return getImpl()->getLeadership();
}

bool UnitViewBase::hasAbility(int abilityId) const
{
    return getImpl()->hasAbility(abilityId);
}

bool UnitViewBase::hasMoveBonus(int groundId) const
{
    return getImpl()->hasMoveBonus(groundId);
}

std::vector<ModifierView> UnitViewBase::getModifiers() const
{
    return getImpl()->getModifiers();
}

} // namespace bindings
