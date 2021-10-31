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

#include "uievent.h"
#include "version.h"
#include <array>

namespace game::UiEventApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::CopyConstructor)0x562081,
        (Api::Destructor)0x562050,
    },
    // Russobit
    Api{
        (Api::CopyConstructor)0x562081,
        (Api::Destructor)0x562050,
    },
    // Gog
    Api{
        (Api::CopyConstructor)0x56181e,
        (Api::Destructor)0x5617ed,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::UiEventApi
