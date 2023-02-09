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

#include "enclayoutcity.h"
#include "version.h"
#include <array>

namespace game::CEncLayoutCityApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Update)0x57b21a,
        (Api::UpdateGroupUi)0x57ba69,
    },
    // Russobit
    Api{
        (Api::Update)0x57b21a,
        (Api::UpdateGroupUi)0x57ba69,
    },
    // Gog
    Api{
        (Api::Update)0x57a8d5,
        (Api::UpdateGroupUi)0x57b124,
    },
    // Scenario Editor
    Api{
        (Api::Update)0x4cc638,
        (Api::UpdateGroupUi)0x4cce87,
    },
}};

static std::array<IEncLayoutVftable*, 4> vftables = {{
    // Akella
    (IEncLayoutVftable*)0x6e8c64,
    // Russobit
    (IEncLayoutVftable*)0x6e8c64,
    // Gog
    (IEncLayoutVftable*)0x6e6c04,
    // Scenario Editor
    (IEncLayoutVftable*)0x5d801c,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

IEncLayoutVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CEncLayoutCityApi
