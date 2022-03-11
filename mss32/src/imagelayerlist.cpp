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

#include "imagelayerlist.h"
#include "version.h"
#include <array>

namespace game::ImageLayerListApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::PushBack)0x522151,
        (Api::Clear)0x522289,
        (Api::AddShieldImageLayer)0x5bca73,
    },
    // Russobit
    Api{
        (Api::PushBack)0x522151,
        (Api::Clear)0x522289,
        (Api::AddShieldImageLayer)0x5bca73,
    },
    // Gog
    Api{
        (Api::PushBack)0x524c10,
        (Api::Clear)0x407a04,
        (Api::AddShieldImageLayer)0x5bbb37,
    },
    // Scenario Editor
    Api{
        (Api::PushBack)0x55e5b7,
        (Api::Clear)0x5541ca,
        (Api::AddShieldImageLayer)0x55d8eb,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::ImageLayerListApi
