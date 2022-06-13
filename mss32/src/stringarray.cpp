/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
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

#include "stringarray.h"
#include "version.h"
#include <array>

namespace game::StringArrayApi {

// clang-format off
static std::array<Api, 4> functions = { {
    // Akella
    Api{
        (Api::Destructor)0x40a63e,
        (Api::Reserve)0x4173e8,
        (Api::PushBack)0x4173ba,
    },
    // Russobit
    Api{
        (Api::Destructor)0x40a63e,
        (Api::Reserve)0x4173e8,
        (Api::PushBack)0x4173ba,
    },
    // Gog
    Api{
        (Api::Destructor)0x40a27d,
        (Api::Reserve)0x416fdc,
        (Api::PushBack)0x416fae,
    },
    // Scenario Editor
    Api{
        (Api::Destructor)0x414f16,
        (Api::Reserve)0x4370a3,
        (Api::PushBack)0x414ea3,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::StringArrayApi
