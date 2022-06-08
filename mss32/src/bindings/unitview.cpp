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
#include "globaldata.h"
#include "log.h"
#include "midunit.h"
#include "modifierview.h"
#include "ummodifier.h"
#include "unitgenerator.h"
#include "unitimplview.h"
#include "unitutils.h"
#include "ussoldier.h"
#include "usunitimpl.h"
#include <fmt/format.h>
#include <sol/sol.hpp>

namespace bindings {

UnitView::UnitView(const game::CMidUnit* unit)
    : unit(unit)
    , unitImpl(nullptr)
{ }

UnitView::UnitView(const game::CMidUnit* unit, const game::IUsUnit* unitImpl)
    : unit(unit)
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
    unit["modifiers"] = sol::property(&UnitView::getModifiers);

    // Leader properties for backward compatibility
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

std::vector<ModifierView> UnitView::getModifiers() const
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    std::vector<ModifierView> result;

    CUmModifier* modifier = nullptr;
    for (auto curr = unit->unitImpl; curr; curr = modifier->data->prev) {
        modifier = (CUmModifier*)dynamicCast(curr, 0, rtti.IUsUnitType, rtti.CUmModifierType, 0);
        if (!modifier)
            break;

        result.push_back(ModifierView{modifier});
    }

    std::reverse(result.begin(), result.end());
    return result;
}

const game::IUsUnit* UnitView::getUnitImpl() const
{
    return unitImpl ? unitImpl : unit->unitImpl;
}

} // namespace bindings
