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

#include "idlist.h"
#include "version.h"
#include <array>

namespace game::IdListApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::SetEmpty)0x56ab57,
        (Api::Add)0x5c9ebd,
        (Api::Remove)0x408017,
    },
    // Russobit
    Api{
        (Api::SetEmpty)0x56ab57,
        (Api::Add)0x5c9ebd,
        (Api::Remove)0x408017,
    },
    // Gog
    Api{
        (Api::SetEmpty)0x42c2c1,
        (Api::Add)0x56aff7,
        (Api::Remove)0x407ca2,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::IdListApi
