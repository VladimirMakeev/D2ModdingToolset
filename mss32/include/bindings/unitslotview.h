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

#ifndef UNITSLOTVIEW_H
#define UNITSLOTVIEW_H

#include "midgardid.h"
#include <optional>

namespace sol {
class state;
}

namespace game {
struct IMidgardObjectMap;
struct CMidUnit;
} // namespace game

namespace bindings {

class UnitView;

class UnitSlotView
{
public:
    UnitSlotView(const game::CMidUnit* unit,
                 const game::IMidgardObjectMap* objectMap,
                 int position,
                 const game::CMidgardID* groupId);
    bool operator==(const UnitSlotView& value) const;

    static void bind(sol::state& lua);

    std::optional<UnitView> getUnitView() const;
    int getPosition() const;
    int getLine() const;
    int getColumn() const;
    bool isFrontline() const;
    bool isBackline() const;
    int getDistance(const UnitSlotView& to) const;

    game::CMidgardID getUnitId() const;
    const game::CMidUnit* getUnit() const;

private:
    const game::CMidUnit* unit;
    const game::IMidgardObjectMap* objectMap;
    int position;
    game::CMidgardID groupId;
};

} // namespace bindings

#endif // UNITSLOTVIEW_H
