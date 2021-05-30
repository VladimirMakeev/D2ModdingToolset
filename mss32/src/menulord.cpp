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

#include "menulord.h"
#include "version.h"
#include <array>

namespace game::CMenuLordApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x4e6368,
        (Api::GetLordFacesByRace)0x4e694a,
        (Api::LoadLordFaceImages)0x4e69ce,
        (Api::GetLordAnimation)0x4e7568,
        (Api::LoadLordAnimation)0x4eb590,
    },
    // Russobit
    Api{
        (Api::Constructor)0x4e6368,
        (Api::GetLordFacesByRace)0x4e694a,
        (Api::LoadLordFaceImages)0x4e69ce,
        (Api::GetLordAnimation)0x4e7568,
        (Api::LoadLordAnimation)0x4eb590,
    },
    // Gog
    Api{
        (Api::Constructor)0x4e5899,
        (Api::GetLordFacesByRace)0x4e5e7b,
        (Api::LoadLordFaceImages)0x4e5eff,
        (Api::GetLordAnimation)0x4e6a67,
        (Api::LoadLordAnimation)0x4eaa42,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMenuLordApi
