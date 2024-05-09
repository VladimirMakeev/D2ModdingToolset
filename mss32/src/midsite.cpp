/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#include "midsite.h"
#include "version.h"
#include <array>

namespace game::CMidSiteApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x601f13,
        (Api::SetData)nullptr,
    },
    // Russobit
    Api{
        (Api::Constructor)0x601f13,
        (Api::SetData)nullptr,
    },
    // Gog
    Api{
        (Api::Constructor)0x600bc6,
        (Api::SetData)nullptr,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x4f3203,
        (Api::SetData)0x4f33a0,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMidSiteApi
