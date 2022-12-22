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

#include "game.h"
#include "unitview.h"
#include "midunit.h"
#include "modifierutils.h"
#include "modifierview.h"
#include "unitimplview.h"
#include "unitmodifier.h"
#include "unitutils.h"
#include "unitviewdummy.h"
#include "ussoldier.h"
#include "usunitimpl.h"
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
    unit["leveledImpl"] = sol::property(&UnitView::getLeveledImpl);
    unit["original"] = sol::property(&UnitView::getOriginal);
    unit["originalModifiers"] = sol::property(&UnitView::getOriginalModifiers);

    // For backward compatibility
    unit["modifiers"] = sol::property(&UnitView::getModifiers);
    unit["type"] = sol::property(&UnitView::getLeaderCategory);
    unit["movement"] = sol::property(&UnitView::getMovement);
    unit["scout"] = sol::property(&UnitView::getScout);
    unit["leadership"] = sol::property(&UnitView::getLeadership);
    unit["hasAbility"] = &UnitView::hasAbility;
    unit["hasMoveBonus"] = &UnitView::hasMoveBonus;
}

std::optional<UnitViewDummy> UnitView::getOriginal() const
{
    if (!unit->transformed) {
        return std::nullopt;
    }

    return UnitViewDummy{unit->id, hooks::getUnitImpl(&unit->origTypeId), unit->origXp,
                         unit->hpBefore, unit->hpBefMax};
}

std::vector<ModifierView> UnitView::getOriginalModifiers() const
{
    std::vector<ModifierView> result;

    for (auto modifierId : unit->origModifiers) {
        auto modifier = hooks::getUnitModifier(&modifierId);
        result.push_back(ModifierView{modifier->data->modifier});
    }

    return result;
}

game::CMidgardID UnitView::getIdInternal() const
{
    return unit->id;
}

const game::IUsUnit* UnitView::getImplInternal() const
{
    return unitImpl ? unitImpl : unit->unitImpl;
}

int UnitView::getXpInternal() const
{
    return unit->currentXp;
}

int UnitView::getHpInternal() const
{
    return unit->currentHp;
}

int UnitView::getHpMaxInternal() const
{
    using namespace game;

    if (unit->transformed && unit->keepHp)
        return unit->hpBefMax;

    auto soldier = gameFunctions().castUnitImplToSoldier(getImplInternal());
    return soldier->vftable->getHitPoints(soldier);
}

} // namespace bindings
