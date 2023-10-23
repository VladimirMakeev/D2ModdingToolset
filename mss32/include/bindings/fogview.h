/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#ifndef FOGVIEW_H
#define FOGVIEW_H

#include "idview.h"

namespace sol {
class state;
}

namespace game {
struct CMidgardMapFog;
}

namespace bindings {

struct Point;

class FogView
{
public:
    FogView(const game::CMidgardMapFog* mapFog);

    static void bind(sol::state& lua);

    IdView getId() const;

    bool getFogByCoordinates(int x, int y) const;
    bool getFogByPoint(const Point& p) const;

private:
    const game::CMidgardMapFog* mapFog;
};

} // namespace bindings

#endif // FOGVIEW_H
