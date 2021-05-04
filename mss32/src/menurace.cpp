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

#include "menurace.h"
#include "version.h"
#include <array>

namespace game::CMenuRaceApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::ScrollButtonCallback)0x4eedcf,
        (Api::ScrollButtonCallback)0x4eefac,
        (Api::UpdateButtons)0x4ee5a4,
        (Api::UpdateRaceDescription)0x4eec47,
        (Api::GetRaceCategory)0x4ee8ec,
        (Api::GetRaceBgndImageName)0x4ee976,
        (Api::GetTransitionAnimationName)0x4eeeb1,
        (Api::SetRacesToSkip)0x4ee3d1,
    },
    // Russobit
    Api{
        (Api::ScrollButtonCallback)0x4eedcf,
        (Api::ScrollButtonCallback)0x4eefac,
        (Api::UpdateButtons)0x4ee5a4,
        (Api::UpdateRaceDescription)0x4eec47,
        (Api::GetRaceCategory)0x4ee8ec,
        (Api::GetRaceBgndImageName)0x4ee976,
        (Api::GetTransitionAnimationName)0x4eeeb1,
        (Api::SetRacesToSkip)0x4ee3d1,
    },
    // Gog
    Api{
        (Api::ScrollButtonCallback)0x4ee217,
        (Api::ScrollButtonCallback)0x4ee3f4,
        (Api::UpdateButtons)0x4ed9ec,
        (Api::UpdateRaceDescription)0x4ee08f,
        (Api::GetRaceCategory)0x4edd34,
        (Api::GetRaceBgndImageName)0x4eddbe,
        (Api::GetTransitionAnimationName)0x4ee2f9,
        (Api::SetRacesToSkip)0x4ed819,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMenuRaceApi
