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

#include "unitview.h"
#include "dynupgrade.h"
#include "game.h"
#include "gameutils.h"
#include "globaldata.h"
#include "log.h"
#include "midunit.h"
#include "unitgenerator.h"
#include "unitimplview.h"
#include "unitutils.h"
#include "ussoldier.h"
#include "usunitimpl.h"
#include <fmt/format.h>
#include <sol/sol.hpp>

namespace bindings {

UnitView::UnitView(const game::CMidUnit* unit, const game::IMidgardObjectMap* objectMap)
    : unit(unit)
    , objectMap(objectMap)
    , unitImpl(nullptr)
{ }

UnitView::UnitView(const game::CMidUnit* unit,
                   const game::IMidgardObjectMap* objectMap,
                   const game::IUsUnit* unitImpl)
    : unit(unit)
    , objectMap(objectMap)
    , unitImpl(unitImpl)
{ }

void UnitView::bind(sol::state& lua)
{
    auto unit = lua.new_usertype<UnitView>("UnitView");
    unit["id"] = sol::property(&UnitView::getId);
    unit["xp"] = sol::property(&UnitView::getXp);
    unit["hp"] = sol::property(&UnitView::getHp);
    unit["hpMax"] = sol::property(&UnitView::getHpMax);
    unit["impl"] = sol::property(&UnitView::getImpl);
    unit["baseImpl"] = sol::property(&UnitView::getBaseImpl);
    unit["group"] = sol::property(&UnitView::getGroup);
    unit["stack"] = sol::property(&UnitView::getStack);

    unit["type"] = sol::property(&UnitView::getCategory);
    unit["movement"] = sol::property(&UnitView::getMovement);
    unit["scout"] = sol::property(&UnitView::getScout);
    unit["leadership"] = sol::property(&UnitView::getLeadership);
    unit["hasAbility"] = &UnitView::hasAbility;
    unit["hasMoveBonus"] = &UnitView::hasMoveBonus;
}

std::optional<UnitImplView> UnitView::getImpl() const
{
    return {getUnitImpl()};
}

std::optional<UnitImplView> UnitView::getBaseImpl() const
{
    return {hooks::getGlobalUnitImpl(unit)};
}

GroupView UnitView::getGroup() const
{
    using namespace game;

    auto groupId = gameFunctions().getStackFortRuinId(&unit->id, objectMap);

    auto group = hooks::getGroup(objectMap, groupId);

    return GroupView{group, objectMap, groupId};
}

std::optional<StackView> UnitView::getStack() const
{
    auto stack = hooks::getStackByUnitId(objectMap, &unit->id);
    if (!stack)
        return std::nullopt;

    return {StackView{stack, objectMap}};
}

IdView UnitView::getId() const
{
    return IdView{unit->id};
}

int UnitView::getXp() const
{
    return unit->currentXp;
}

int UnitView::getHp() const
{
    return unit->currentHp;
}

int UnitView::getHpMax() const
{
    using namespace game;

    if (unit->transformed && unit->keepHp)
        return unit->hpBefMax;

    auto soldier = gameFunctions().castUnitImplToSoldier(getUnitImpl());
    return soldier->vftable->getHitPoints(soldier);
}

int UnitView::getCategory() const
{
    // Returns leader category for backward compatibility.
    return getImpl()->getLeaderCategory();
}

int UnitView::getMovement() const
{
    return getImpl()->getMovement();
}

int UnitView::getScout() const
{
    return getImpl()->getScout();
}

int UnitView::getLeadership() const
{
    return getImpl()->getLeadership();
}

bool UnitView::hasAbility(int abilityId) const
{
    return getImpl()->hasAbility(abilityId);
}

bool UnitView::hasMoveBonus(int groundId) const
{
    return getImpl()->hasMoveBonus(groundId);
}

const game::IUsUnit* UnitView::getUnitImpl() const
{
    return unitImpl ? unitImpl : unit->unitImpl;
}

} // namespace bindings
