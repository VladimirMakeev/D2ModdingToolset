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

#include "stringpairarrayptr.h"
#include "version.h"
#include <array>

namespace game::StringPairArrayPtrApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x6092ac,
        (Api::SetData)0x5e475a,
    },
    // Russobit
    Api{
        (Api::Constructor)0x6092ac,
        (Api::SetData)0x5e475a,
    },
    // Gog
    Api{
        (Api::Constructor)0x607d76,
        (Api::SetData)0x5e34a4,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x503f4b,
        (Api::SetData)0x4dcd74,
    },
}};

// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::StringPairArrayPtrApi
