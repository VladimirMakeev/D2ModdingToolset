/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "ummodifier.h"
#include "version.h"
#include <array>

namespace game::CUmModifierApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x5a9dd4,
        (Api::CopyConstructor)0x5a9e37,
        (Api::Destructor)0x5a9e70,
        (Api::SetPrev)0x5a9ea9,
    },
    // Russobit
    Api{
        (Api::Constructor)0x5a9dd4,
        (Api::CopyConstructor)0x5a9e37,
        (Api::Destructor)0x5a9e70,
        (Api::SetPrev)0x5a9ea9,
    },
    // Gog
    Api{
        (Api::Constructor)0x5a906a,
        (Api::CopyConstructor)0x5a90cd,
        (Api::Destructor)0x5a9106,
        (Api::SetPrev)0x5a9138,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x53ec7e,
        (Api::CopyConstructor)0x53ece1,
        (Api::Destructor)0x53ed1a,
        (Api::SetPrev)0x53ed4c,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CUmModifierApi
