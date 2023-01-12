/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#include "umstack.h"
#include "version.h"
#include <array>

namespace game::CUmStackApi {

// clang-format off
static std::array<Vftable, 4> vftables = {{
    // Akella
    Vftable{
        (IUsUnitVftable*)0x6ecd84,
        (IUsStackLeaderVftable*)0x6ecd54,
        (CUmModifierVftable*)0x6ecd24,
    },
    // Russobit
    Vftable{
        (IUsUnitVftable*)0x6ecd84,
        (IUsStackLeaderVftable*)0x6ecd54,
        (CUmModifierVftable*)0x6ecd24,
    },
    // Gog
    Vftable{
        (IUsUnitVftable*)0x6ead24,
        (IUsStackLeaderVftable*)0x6eacf4,
        (CUmModifierVftable*)0x6eacc4,
    },
    // Scenario Editor
    Vftable{
        (IUsUnitVftable*)0x5e0f4c,
        (IUsStackLeaderVftable*)0x5e0f1c,
        (CUmModifierVftable*)0x5e0eec,
    },
}};
// clang-format on

Vftable& vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CUmStackApi
