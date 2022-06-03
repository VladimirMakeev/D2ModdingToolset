/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#include "stackview.h"
#include "game.h"
#include "midgardobjectmap.h"
#include "midstack.h"
#include "midsubrace.h"
#include "unitview.h"
#include <sol/sol.hpp>

namespace bindings {

StackView::StackView(const game::CMidStack* stack, const game::IMidgardObjectMap* objectMap)
    : stack{stack}
    , objectMap{objectMap}
{ }

void StackView::bind(sol::state& lua)
{
    auto stackView = lua.new_usertype<StackView>("StackView");
    stackView["group"] = sol::property(&StackView::getGroup);
    stackView["leader"] = sol::property(&StackView::getLeader);
    stackView["movement"] = sol::property(&StackView::getMovement);
    stackView["subrace"] = sol::property(&StackView::getSubrace);
    stackView["inside"] = sol::property(&StackView::isInside);
    stackView["invisible"] = sol::property(&StackView::isInvisible);
}

GroupView StackView::getGroup() const
{
    return GroupView{&stack->group, objectMap, &stack->id};
}

std::optional<UnitView> StackView::getLeader() const
{
    auto leaderUnit{game::gameFunctions().findUnitById(objectMap, &stack->leaderId)};
    if (!leaderUnit) {
        return std::nullopt;
    }

    return UnitView{leaderUnit, objectMap};
}

int StackView::getMovement() const
{
    return stack->movement;
}

int StackView::getSubrace() const
{
    auto obj{objectMap->vftable->findScenarioObjectById(objectMap, &stack->subraceId)};
    auto subrace{static_cast<const game::CMidSubRace*>(obj)};

    return subrace ? static_cast<int>(subrace->subraceCategory.id) : 0;
}

bool StackView::isInside() const
{
    return stack->insideId != game::emptyId;
}

bool StackView::isInvisible() const
{
    return stack->invisible;
}

} // namespace bindings
