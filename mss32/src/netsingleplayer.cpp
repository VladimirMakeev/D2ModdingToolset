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

#include "netsingleplayer.h"
#include "version.h"
#include <array>

namespace game::CNetSinglePlayerApi {

// clang-format off
static std::array<IMqNetPlayerVftable*, 4> vftables = {{
    // Akella
    (IMqNetPlayerVftable*)0x6e6bf4,
    // Russobit
    (IMqNetPlayerVftable*)0x6e6bf4,
    // Gog
    (IMqNetPlayerVftable*)0x6e4b94,
    // Scenario Editor
    (IMqNetPlayerVftable*)nullptr,
}};
// clang-format on

IMqNetPlayerVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CNetSinglePlayerApi
