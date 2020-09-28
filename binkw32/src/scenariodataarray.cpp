/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
 * Copyright (C) 2020 Vladimir Makeev.
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

#include "scenariodataarray.h"
#include "version.h"
#include <array>

namespace game::ScenarioDataArrayApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::PushBack)0x60613f,
        (Api::GetElement)0x606051
    },
    // Russobit
    Api{
        (Api::PushBack)0x60613f,
        (Api::GetElement)0x606051
    },
    // Gog
    Api{
        (Api::PushBack)0x604c4c,
        (Api::GetElement)0x604b5e
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::ScenarioDataArrayApi
