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

#include "unitviewdummy.h"
#include "modifierview.h"
#include "unitimplview.h"
#include <sol/sol.hpp>

namespace bindings {

UnitViewDummy::UnitViewDummy(const game::CMidgardID& id,
                             const game::IUsUnit* impl,
                             int xp,
                             int hp,
                             int hpMax)
    : id(id)
    , impl(impl)
    , xp(xp)
    , hp(hp)
    , hpMax(hpMax)
{ }

void UnitViewDummy::bind(sol::state& lua)
{
    auto unit = lua.new_usertype<UnitViewDummy>("UnitViewDummy");
    unit["id"] = sol::property(&UnitViewDummy::getId);
    unit["xp"] = sol::property(&UnitViewDummy::getXp);
    unit["hp"] = sol::property(&UnitViewDummy::getHp);
    unit["hpMax"] = sol::property(&UnitViewDummy::getHpMax);
    unit["impl"] = sol::property(&UnitViewDummy::getImpl);
    unit["baseImpl"] = sol::property(&UnitViewDummy::getBaseImpl);
    unit["leveledImpl"] = sol::property(&UnitViewDummy::getLeveledImpl);

    // For backward compatibility
    unit["modifiers"] = sol::property(&UnitViewDummy::getModifiers);
    unit["type"] = sol::property(&UnitViewDummy::getLeaderCategory);
    unit["movement"] = sol::property(&UnitViewDummy::getMovement);
    unit["scout"] = sol::property(&UnitViewDummy::getScout);
    unit["leadership"] = sol::property(&UnitViewDummy::getLeadership);
    unit["hasAbility"] = &UnitViewDummy::hasAbility;
    unit["hasMoveBonus"] = &UnitViewDummy::hasMoveBonus;
}

game::CMidgardID UnitViewDummy::getIdInternal() const
{
    return id;
}

const game::IUsUnit* UnitViewDummy::getImplInternal() const
{
    return impl;
}

int UnitViewDummy::getXpInternal() const
{
    return xp;
}

int UnitViewDummy::getHpInternal() const
{
    return hp;
}

int UnitViewDummy::getHpMaxInternal() const
{
    return hpMax;
}

} // namespace bindings
