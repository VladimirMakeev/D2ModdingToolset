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

#include "tileview.h"
#include "midgardmapblock.h"
#include <sol/sol.hpp>

namespace bindings {

TileView::TileView(std::uint32_t tile)
    : tile(tile)
{ }

void TileView::bind(sol::state& lua)
{
    auto tileView = lua.new_usertype<TileView>("Tile");
    tileView["terrain"] = sol::property(&TileView::getTerrain);
    tileView["ground"] = sol::property(&TileView::getGround);
}

int TileView::getTerrain() const
{
    return static_cast<int>(game::tileTerrain(tile));
}

int TileView::getGround() const
{
    return static_cast<int>(game::tileGround(tile));
}

} // namespace bindings
