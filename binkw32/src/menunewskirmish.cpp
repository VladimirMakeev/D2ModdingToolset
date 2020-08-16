/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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
        (Api::ButtonCallback)0x4dcf9e,
        (Api::ButtonCallback)0x4e9d2a,
        (Api::CreateButtonFunctor)0x4ea842,
        (Api::ListBoxCallback)0x4e9cb3,
        (Api::CreateListBoxFunctor)0x4ea8da,
        (Api::ListBoxDisplayTextCallback)0x4e9c1a,
        (Api::CreateListBoxDisplayTextFunctor)0x4ea88e,
        (Api::UpdateScenarioUi)0x4e987c
    },
    // Russobit
    Api{},
    // Gog
    Api{}
}};

static std::array<Vftable*, 3> vftables = {{
    // Akella
    (Vftable*)0x6dec9c,
    // Russobit
    (Vftable*)0x0,
    // Gog
    (Vftable*)0x0
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
