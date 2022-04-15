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

#include "musicfader.h"
#include "version.h"
#include <array>

namespace game::CMusicFaderApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Get)0x5b065e,
        (Api::HasEventId)0x5b0c33,
        (Api::Callback)0x5b0b6d,
    },
    // Russobit
    Api{
        (Api::Get)0x5b065e,
        (Api::HasEventId)0x5b0c33,
        (Api::Callback)0x5b0b6d,
    },
    // Gog
    Api{
        (Api::Get)0x5af952,
        (Api::HasEventId)0x5aff27,
        (Api::Callback)0x5afe61,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMusicFaderApi
