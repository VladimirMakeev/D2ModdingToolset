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

#ifndef GROUPVIEW_H
#define GROUPVIEW_H

#include "midgardid.h"
#include <vector>

namespace sol {
class state;
}

namespace game {
struct CMidUnitGroup;
struct IMidgardObjectMap;
} // namespace game

namespace bindings {

struct IdView;
class UnitView;
class UnitSlotView;

class GroupView
{
public:
    using GroupSlots = std::vector<UnitSlotView>;
    using GroupUnits = std::vector<UnitView>;

    GroupView(const game::CMidUnitGroup* group,
              const game::IMidgardObjectMap* objectMap,
              const game::CMidgardID* groupId);

    static void bind(sol::state& lua);

    /** Returns group as array of 6 unit slots. */
    GroupSlots getSlots() const;

    GroupUnits getUnits() const;

    bool hasUnit(const bindings::UnitView& unit) const;
    bool hasUnitById(const bindings::IdView& unitId) const;

protected:
    const game::CMidUnitGroup* group;
    const game::IMidgardObjectMap* objectMap;
    game::CMidgardID groupId;
};

} // namespace bindings

#endif // GROUPVIEW_H
