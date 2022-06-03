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

#ifndef STACKVIEW_H
#define STACKVIEW_H

#include "groupview.h"
#include "idview.h"
#include <optional>
#include <vector>

namespace game {
struct CMidStack;
struct IMidgardObjectMap;

enum EquippedItemIdx;
} // namespace game

namespace bindings {

class UnitView;
class ItemView;

class StackView
{
public:
    StackView(const game::CMidStack* stack, const game::IMidgardObjectMap* objectMap);

    static void bind(sol::state& lua);

    IdView getId() const;
    /** Returns stack units as a group. */
    GroupView getGroup() const;
    /** Returns stack leader. */
    std::optional<UnitView> getLeader() const;
    /** Returns stack current movement points. */
    int getMovement() const;
    /** Returns stack subrace category id. */
    int getSubrace() const;

    bool isInside() const;

    bool isInvisible() const;

    std::vector<ItemView> getInventoryItems() const;
    std::optional<ItemView> getLeaderEquippedItem(const game::EquippedItemIdx& idx) const;

private:
    const game::CMidStack* stack;
    const game::IMidgardObjectMap* objectMap;
};

} // namespace bindings

#endif // STACKVIEW_H
