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

#include "idvector.h"
#include "version.h"
#include <array>

namespace game::IdVectorApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Destructor)0x410dc1,
        (Api::PushBack)0x4118f0,
        (Api::Reserve)0x41683d,
        (Api::Copy)0x46c081,
    },
    // Russobit
    Api{
        (Api::Destructor)0x410dc1,
        (Api::PushBack)0x4118f0,
        (Api::Reserve)0x41683d,
        (Api::Copy)0x46c081,
    },
    // Gog
    Api{
        (Api::Destructor)0x41099f,
        (Api::PushBack)0x4114ce,
        (Api::Reserve)0x416517,
        (Api::Copy)0x46ba06,
    },
    // Scenario Editor
    Api{
        (Api::Destructor)0x414ed1,
        (Api::PushBack)0x4151ff,
        (Api::Reserve)0x4152eb,
        (Api::Copy)0x415269,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::IdVectorApi
