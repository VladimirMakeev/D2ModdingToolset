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

#include "d2string.h"
#include "version.h"
#include <array>

namespace game::StringApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::InitFromString)0x403d80,
        (Api::InitFromStringN)0x403f62,
        (Api::Free)0x401418,
        (Api::Append)0x401453,
        (Api::AppendChar)0x4248a4,
        (Api::CStr)0x403dc6,
    },
    // Russobit
    Api{
        (Api::InitFromString)0x403d80,
        (Api::InitFromStringN)0x403f62,
        (Api::Free)0x401418,
        (Api::Append)0x401453,
        (Api::AppendChar)0x4248a4,
        (Api::CStr)0x403dc6,
    },
    // Gog
    Api{
        (Api::InitFromString)0x403ac6,
        (Api::InitFromStringN)0x403c8b,
        (Api::Free)0x40106a,
        (Api::Append)0x4010a5,
        (Api::AppendChar)0x4243a5,
        (Api::CStr)0x403b0c,
    },
    // Scenario Editor
    Api{
        (Api::InitFromString)0x402f6c,
        (Api::InitFromStringN)0x403079,
        (Api::Free)0x402fb2,
        (Api::Append)0x4043ac,
        (Api::AppendChar)0x472022,
        (Api::CStr)0x405141,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::StringApi
