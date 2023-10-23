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
#include "fortview.h"
#include "game.h"
#include "gameutils.h"
#include "itemview.h"
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
    stackView["id"] = sol::property(&StackView::getId);
    stackView["position"] = sol::property(&StackView::getPosition);
    stackView["owner"] = sol::property(&StackView::getOwner);
    stackView["inside"] = sol::property(&StackView::getInside);
    stackView["group"] = sol::property(&StackView::getGroup);
    stackView["leader"] = sol::property(&StackView::getLeader);
    stackView["movement"] = sol::property(&StackView::getMovement);
    stackView["subrace"] = sol::property(&StackView::getSubrace);
    stackView["invisible"] = sol::property(&StackView::isInvisible);
    stackView["battlesWon"] = sol::property(&StackView::getBattlesWon);

    stackView["inventory"] = sol::property(&StackView::getInventoryItems);
    stackView["getEquippedItem"] = &StackView::getLeaderEquippedItem;
}

IdView StackView::getId() const
{
    return IdView{stack->id};
}

Point StackView::getPosition() const
{
    return Point{stack->position};
}

std::optional<PlayerView> StackView::getOwner() const
{
    auto player = hooks::getPlayer(objectMap, &stack->ownerId);
    if (!player) {
        return std::nullopt;
    }

    return PlayerView{player, objectMap};
}

std::optional<FortView> StackView::getInside() const
{
    auto fort = hooks::getFort(objectMap, &stack->insideId);
    if (!fort) {
        return std::nullopt;
    }

    return {FortView{fort, objectMap}};
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

    return UnitView{leaderUnit};
}

int StackView::getMovement() const
{
    return stack->movement;
}

int StackView::getSubrace() const
{
    auto obj{objectMap->vftable->findScenarioObjectById(objectMap, &stack->subraceId)};
    auto subrace{static_cast<const game::CMidSubRace*>(obj)};

    return subrace ? static_cast<int>(subrace->subraceCategory.id) : game::emptyCategoryId;
}

bool StackView::isInvisible() const
{
    return stack->invisible;
}

int StackView::getBattlesWon() const
{
    return stack->nbBattle;
}

std::vector<ItemView> StackView::getInventoryItems() const
{
    const auto& items = stack->inventory.items;
    const auto count = items.end - items.bgn;

    std::vector<ItemView> result;
    result.reserve(count);

    for (const game::CMidgardID* it = items.bgn; it != items.end; it++) {
        result.push_back(ItemView{it, objectMap});
    }

    return result;
}

std::optional<ItemView> StackView::getLeaderEquippedItem(const game::EquippedItemIdx& idx) const
{
    const auto& items = stack->leaderEquippedItems;
    const auto count = items.end - items.bgn;
    if (idx < 0 || idx >= count)
        return std::nullopt;

    auto itemId = stack->leaderEquippedItems.bgn[idx];
    if (itemId == game::emptyId)
        return std::nullopt;

    return {ItemView{&itemId, objectMap}};
}

} // namespace bindings
