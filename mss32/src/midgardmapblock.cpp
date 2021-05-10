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

#include "midgardmapblock.h"
#include "version.h"
#include <array>

namespace game::CMidgardMapBlockApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Stream)0x5f48ab,
        (Api::CountTerrainCoverage)0x5f46fc,
    },
    // Russobit
    Api{
        (Api::Stream)0x5f48ab,
        (Api::CountTerrainCoverage)0x5f46fc,
    },
    // Gog
    Api{
        (Api::Stream)0x5f3578,
        (Api::CountTerrainCoverage)0x5f33c9,
    },
    // Scenario Editor
    Api{
        (Api::Stream)0x5027f1,
        (Api::CountTerrainCoverage)nullptr,
    },
}};

// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMidgardMapBlockApi
