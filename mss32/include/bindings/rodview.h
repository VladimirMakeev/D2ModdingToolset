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

#ifndef RODVIEW_H
#define RODVIEW_H

#include "idview.h"
#include "point.h"
#include <optional>

namespace sol {
class state;
}

namespace game {
struct CMidRod;
struct IMidgardObjectMap;
} // namespace game

namespace bindings {

class PlayerView;

class RodView
{
public:
    RodView(const game::CMidRod* rod, const game::IMidgardObjectMap* objectMap);

    static void bind(sol::state& lua);

    IdView getId() const;
    Point getPosition() const;
    std::optional<PlayerView> getOwner() const;

private:
    const game::CMidRod* rod;
    const game::IMidgardObjectMap* objectMap;
};

} // namespace bindings

#endif // RODVIEW_H
