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

#include "campaignstream.h"
#include "version.h"
#include <array>

namespace game::CampaignStreamApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x609619,
        (Api::HasErrors)0x5e5099,
        (Api::Destructor)0x609695,
    },
    // Russobit
    Api{
        (Api::Constructor)0x609619,
        (Api::HasErrors)0x5e5099,
        (Api::Destructor)0x609695,
    },
    // Gog
    Api{
        (Api::Constructor)0x6080e3,
        (Api::HasErrors)0x5e3db6,
        (Api::Destructor)0x60815f,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x5042b8,
        (Api::HasErrors)0x4dc307,
        (Api::Destructor)0x504334,
    },
}};

// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CampaignStreamApi
