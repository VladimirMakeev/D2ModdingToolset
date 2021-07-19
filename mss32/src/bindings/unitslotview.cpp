/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#include "unitslotview.h"
#include "game.h"
#include "midunit.h"
#include "unitview.h"
#include <sol/sol.hpp>

namespace bindings {

UnitSlotView::UnitSlotView(const game::CMidUnit* unit,
                           int position,
                           const game::CMidgardID* groupId)
    : unit(unit)
    , position(position)
    , groupId(*groupId)
{ }

bool UnitSlotView::operator==(const UnitSlotView& value) const
{
    return position == value.position && groupId == value.groupId;
}

void UnitSlotView::bind(sol::state& lua)
{
    auto slot = lua.new_usertype<UnitSlotView>("UnitSlot", "distance", &getDistance,
                                               sol::meta_function::equal_to, &operator==);
    slot["unit"] = sol::property(&getUnitView);
    slot["position"] = sol::property(&getPosition);
    slot["line"] = sol::property(&getLine);
    slot["column"] = sol::property(&getColumn);
    slot["frontline"] = sol::property(&isFrontline);
    slot["backline"] = sol::property(&isBackline);
    lua.set_function("distance", &getDistance);
}

std::optional<UnitView> UnitSlotView::getUnitView() const
{
    return {UnitView(unit)};
}

int UnitSlotView::getPosition() const
{
    return position;
}

int UnitSlotView::getLine() const
{
    return position % 2;
}

int UnitSlotView::getColumn() const
{
    return position / 2;
}

bool UnitSlotView::isFrontline() const
{
    return position % 2 == 0;
}

bool UnitSlotView::isBackline() const
{
    return position % 2 != 0;
}

int UnitSlotView::getDistance(const UnitSlotView& to) const
{
    using namespace game;

    const auto& fn = gameFunctions();
    return fn.getUnitPositionDistance(position, to.position, groupId == to.groupId);
}

game::CMidgardID UnitSlotView::getUnitId() const
{
    using namespace game;

    return unit ? unit->unitId : emptyId;
}

const game::CMidUnit* UnitSlotView::getUnit() const
{
    return unit;
}

} // namespace bindings
