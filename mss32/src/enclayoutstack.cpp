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

#include "enclayoutstack.h"
#include "version.h"
#include <array>

namespace game::CEncLayoutStackApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Update)0x57c814,
    },
    // Russobit
    Api{
        (Api::Update)0x57c814,
    },
    // Gog
    Api{
        (Api::Update)0x57becf,
    },
    // Scenario Editor
    Api{
        (Api::Update)0x4cdc32,
    },
}};

static std::array<CEncLayoutStackVftable*, 4> vftables = {{
    // Akella
    (CEncLayoutStackVftable*)0x6e8dec,
    // Russobit
    (CEncLayoutStackVftable*)0x6e8dec,
    // Gog
    (CEncLayoutStackVftable*)0x6e6d8c,
    // Scenario Editor
    (CEncLayoutStackVftable*)0x5d81a4,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

CEncLayoutStackVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CEncLayoutStackApi
