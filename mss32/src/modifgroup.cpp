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

#include "modifgroup.h"
#include "version.h"
#include <array>

namespace game::LModifGroupApi {

// clang-format off
static std::array<LModifGroupVftable*, 4> vftables = {{
    // Akella
    (LModifGroupVftable*)0x6ea68c,
    // Russobit
    (LModifGroupVftable*)0x6ea68c,
    // Gog
    (LModifGroupVftable*)0x6e862c,
    // Scenario Editor
    (LModifGroupVftable*)0x5defa4,
}};
// clang-format on

const LModifGroupVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::LModifGroupApi
