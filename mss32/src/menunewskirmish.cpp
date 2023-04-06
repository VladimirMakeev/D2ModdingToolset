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

#include "menunewskirmish.h"
#include "version.h"
#include <array>

namespace game::CMenuNewSkirmishApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x4e9697,
        (CMenuBaseApi::Api::ButtonCallback)0x4e9d2a,
        (Api::UpdateScenarioUi)0x4e987c,
    },
    // Russobit
    Api{
        (Api::Constructor)0x4e9697,
        (CMenuBaseApi::Api::ButtonCallback)0x4e9d2a,
        (Api::UpdateScenarioUi)0x4e987c,
    },
    // Gog
    Api{
        (Api::Constructor)0x4e8b32,
        (CMenuBaseApi::Api::ButtonCallback)0x4e91c5,
        (Api::UpdateScenarioUi)0x4e8d17,
    },
}};

static std::array<Vftable*, 3> vftables = {{
    // Akella
    (Vftable*)0x6dec9c,
    // Russobit
    (Vftable*)0x6dec9c,
    // Gog
    (Vftable*)0x6dcc44,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

Vftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMenuNewSkirmishApi
