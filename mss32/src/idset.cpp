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

#include "idset.h"
#include "version.h"
#include <array>

namespace game::IdSetApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Insert)0x525755,
        (Api::Find)0x59f52d,
    },
    // Russobit
    Api{
        (Api::Insert)0x525755,
        (Api::Find)0x59f52d,
    },
    // Gog
    Api{
        (Api::Insert)0x524c35,
        (Api::Find)0x53207e,
    },
    // Scenario Editor
    Api{
        (Api::Insert)0x408551,
        (Api::Find)0x54770d,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::IdSetApi
