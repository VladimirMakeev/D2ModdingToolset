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
#include "unitgenerator.h"
#include "unitimplview.h"
#include "unitutils.h"
#include "ussoldier.h"
#include "usunitimpl.h"
#include "utils.h"
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
    auto unit = lua.new_usertype<UnitView>("Unit");
    unit["xp"] = sol::property(&UnitView::getXp);
    unit["hp"] = sol::property(&UnitView::getHp);
    unit["hpMax"] = sol::property(&UnitView::getHpMax);
    unit["impl"] = sol::property(&UnitView::getImpl);
    unit["baseImpl"] = sol::property(&UnitView::getBaseImpl);
    unit["baseUnit"] = sol::property(&UnitView::getBaseUnit);
}

std::optional<UnitImplView> UnitView::getImpl() const
{
    return {getUnitImpl()};
}

std::optional<UnitImplView> UnitView::getBaseImpl() const
{
    return {hooks::getGlobalUnitImpl(unit)};
}

std::optional<UnitImplView> UnitView::getBaseUnit() const
{
    using namespace game;

    auto soldier = gameFunctions().castUnitImplToSoldier(getUnitImpl());
    auto baseUnitImplId = soldier->vftable->getBaseUnitImplId(soldier);
    if (*baseUnitImplId == emptyId)
        return std::nullopt;

    auto globalUnitImpl = hooks::getGlobalUnitImpl(baseUnitImplId);
    if (!globalUnitImpl)
        return std::nullopt;

    return {globalUnitImpl};
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

const game::IUsUnit* UnitView::getUnitImpl() const
{
    return unitImpl ? unitImpl : unit->unitImpl;
}

} // namespace bindings
