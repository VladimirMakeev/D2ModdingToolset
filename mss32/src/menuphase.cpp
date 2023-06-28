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

#include "menuphase.h"
#include "version.h"
#include <array>

namespace game::CMenuPhaseApi {

// clang-format off
std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x4ea926,
        (Api::SetTransition)0x4eb20c,
        (Api::DoTransition)0x4eae89,
        (Api::ShowFullScreenAnimation)0x4ead7e,
        (Api::SwitchToMenu)0x4eae4d,
        (Api::ShowTransition)0x4ebcd3,
        (Api::SwitchToMenu)0x4eb16c,
        (Api::ShowTransition)0x4ebd9d,
        (Api::ShowTransition)0x4ebe47,
        (Api::ShowTransition)0x4ebf3a,
        (Api::SwitchToMenu)0x4ec23e,
        (Api::ShowTransition)0x4ec340,
        (Api::SwitchToMenu)0x4ec398,
        (Api::ShowTransition)0x4ec3fe,
        (Api::SwitchToMenu)0x4ec079,
        (Api::SwitchToMenu)0x4ec0f2,
        (Api::SwitchToMenu)0x4ec124,
        (Api::SwitchToMenu)0x4ec1b9,
        (Api::SwitchToMenu)0x4ec1e7,
        (Api::SwitchToMenu)0x4ec36a,
        (Api::SwitchToMenu)0x4ec41b,
        (Api::SwitchToMenu)0x4ec449,
        (Api::ShowTransition)0x4ec46f,
        (Api::SwitchToMenu)0x4ebe19,
        (Api::SwitchToMenu)0x4ebe76,
        (Api::ShowTransition)0x4ebea4,
        (Api::SwitchToMenu)0x4ebf0c,
        (Api::SwitchToMenu)0x4ebf90,
        (Api::SwitchToMenu)0x4ebfbe,
        (Api::SwitchToMenu)0x4ebfec,
        (Api::SwitchToMenu)0x4ec009,
        (Api::SwitchToMenu)0x4ec037,
        (Api::SwitchToMenu)0x4ec4e0,
        (Api::SwitchToMenu)0x4ebbf4,
        (Api::SetString)0x4eba33,
        (Api::SetCampaignId)0x4ebb97,
        (Api::SetString)0x4ebbac,
        (Api::SetString)0x4ebbd0,
    },
    // Russobit
    Api{
        (Api::Constructor)0x4ea926,
        (Api::SetTransition)0x4eb20c,
        (Api::DoTransition)0x4eae89,
        (Api::ShowFullScreenAnimation)0x4ead7e,
        (Api::SwitchToMenu)0x4eae4d,
        (Api::ShowTransition)0x4ebcd3,
        (Api::SwitchToMenu)0x4eb16c,
        (Api::ShowTransition)0x4ebd9d,
        (Api::ShowTransition)0x4ebe47,
        (Api::ShowTransition)0x4ebf3a,
        (Api::SwitchToMenu)0x4ec23e,
        (Api::ShowTransition)0x4ec340,
        (Api::SwitchToMenu)0x4ec398,
        (Api::ShowTransition)0x4ec3fe,
        (Api::SwitchToMenu)0x4ec079,
        (Api::SwitchToMenu)0x4ec0f2,
        (Api::SwitchToMenu)0x4ec124,
        (Api::SwitchToMenu)0x4ec1b9,
        (Api::SwitchToMenu)0x4ec1e7,
        (Api::SwitchToMenu)0x4ec36a,
        (Api::SwitchToMenu)0x4ec41b,
        (Api::SwitchToMenu)0x4ec449,
        (Api::ShowTransition)0x4ec46f,
        (Api::SwitchToMenu)0x4ebe19,
        (Api::SwitchToMenu)0x4ebe76,
        (Api::ShowTransition)0x4ebea4,
        (Api::SwitchToMenu)0x4ebf0c,
        (Api::SwitchToMenu)0x4ebf90,
        (Api::SwitchToMenu)0x4ebfbe,
        (Api::SwitchToMenu)0x4ebfec,
        (Api::SwitchToMenu)0x4ec009,
        (Api::SwitchToMenu)0x4ec037,
        (Api::SwitchToMenu)0x4ec4e0,
        (Api::SwitchToMenu)0x4ebbf4,
        (Api::SetString)0x4eba33,
        (Api::SetCampaignId)0x4ebb97,
        (Api::SetString)0x4ebbac,
        (Api::SetString)0x4ebbd0,
    },
    // Gog
    Api{
        (Api::Constructor)0x4e9dd8,
        (Api::SetTransition)0x4ea6be,
        (Api::DoTransition)0x4ea33b,
        (Api::ShowFullScreenAnimation)0x4ea230,
        (Api::SwitchToMenu)0x4ea2ff,
        (Api::ShowTransition)0x4eb193,
        (Api::SwitchToMenu)0x4ea61e,
        (Api::ShowTransition)0x4eb25d,
        (Api::ShowTransition)0x4eb307,
        (Api::ShowTransition)0x4eb3fa,
        (Api::SwitchToMenu)0x4eb6fe,
        (Api::ShowTransition)0x4eb800,
        (Api::SwitchToMenu)0x4eb858,
        (Api::ShowTransition)0x4eb8be,
        (Api::SwitchToMenu)0x4eb539,
        (Api::SwitchToMenu)0x4eb5b2,
        (Api::SwitchToMenu)0x4eb5e4,
        (Api::SwitchToMenu)0x4eb679,
        (Api::SwitchToMenu)0x4eb6a7,
        (Api::SwitchToMenu)0x4eb82a,
        (Api::SwitchToMenu)0x4eb8db,
        (Api::SwitchToMenu)0x4eb909,
        (Api::ShowTransition)0x4eb92f,
        (Api::SwitchToMenu)0x4eb2d9,
        (Api::SwitchToMenu)0x4eb336,
        (Api::ShowTransition)0x4eb364,
        (Api::SwitchToMenu)0x4eb3cc,
        (Api::SwitchToMenu)0x4eb450,
        (Api::SwitchToMenu)0x4eb47e,
        (Api::SwitchToMenu)0x4eb4ac,
        (Api::SwitchToMenu)0x4eb4c9,
        (Api::SwitchToMenu)0x4eb4f7,
        (Api::SwitchToMenu)0x4eb9a0,
        (Api::SwitchToMenu)0x4eb0b4,
        (Api::SetString)0x4eaeec,
        (Api::SetCampaignId)0x4eb050,
        (Api::SetString)0x4eb06c,
        (Api::SetString)0x4eb090,
    },
}};

static std::array<IMqNetSystemVftable*, 3> vftables = {{
    // Akella
    (IMqNetSystemVftable*)0x6df0a4,
    // Russobit
    (IMqNetSystemVftable*)0x6df0a4,
    // Gog
    (IMqNetSystemVftable*)0x6dd04c,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

IMqNetSystemVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMenuPhaseApi
