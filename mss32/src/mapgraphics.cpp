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

#include "mapgraphics.h"
#include "version.h"
#include <array>

namespace game::MapGraphicsApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::GetMapGraphics)0x540e28,
        (Api::SetMapGraphics)0x407f37,
        (Api::GetTileIndex)0x541e15,
        (Api::StoreBlackTiles)0x541e65,
    },
    // Russobit
    Api{
        (Api::GetMapGraphics)0x540e28,
        (Api::SetMapGraphics)0x407f37,
        (Api::GetTileIndex)0x541e15,
        (Api::StoreBlackTiles)0x541e65,
    },
    // Gog
    Api{
        (Api::GetMapGraphics)0x5404d9,
        (Api::SetMapGraphics)0x407be5,
        (Api::GetTileIndex)0x5414c5,
        (Api::StoreBlackTiles)0x541515,
    },
    // Scenario Editor
    Api{
        (Api::GetMapGraphics)nullptr,
        (Api::SetMapGraphics)nullptr,
        (Api::GetTileIndex)nullptr,
        (Api::StoreBlackTiles)nullptr,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::MapGraphicsApi
