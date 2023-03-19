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

#include "aigiveitemsaction.h"
#include "version.h"
#include <array>

namespace game::CAiGiveItemsActionApi {

// clang-format off
static std::array<Vftable, 4> vftables = {{
    // Akella
    Vftable{
        (IAiTacticActionVftable*)0x6d2a24,
        (IAiReactionVftable*)0x6d29b4,
    },
    // Russobit
    Vftable{
        (IAiTacticActionVftable*)0x6d2a24,
        (IAiReactionVftable*)0x6d29b4,
    },
    // Gog
    Vftable{
        (IAiTacticActionVftable*)0x6d09c4,
        (IAiReactionVftable*)0x6d0954,
    },
    // Scenario Editor
    Vftable{
        (IAiTacticActionVftable*)nullptr,
        (IAiReactionVftable*)nullptr,
    },
}};
// clang-format on

const Vftable& vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CAiGiveItemsActionApi
