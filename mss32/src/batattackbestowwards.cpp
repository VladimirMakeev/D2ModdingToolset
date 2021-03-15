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

#include "batattackbestowwards.h"
#include "version.h"
#include <array>

namespace game::CBatAttackBestowWardsApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (IBatAttackVftable::CanPerform)0x65e5f9,
        (IBatAttackVftable::OnAttack)0x65eaa8,
    },
    // Russobit
    Api{
        (IBatAttackVftable::CanPerform)0x65e5f9,
        (IBatAttackVftable::OnAttack)0x65eaa8,
    },
    // Gog
    Api{
        (IBatAttackVftable::CanPerform)0x65d079,
        (IBatAttackVftable::OnAttack)0x65d528,
    },
    // Scenario Editor
    Api{
        (IBatAttackVftable::CanPerform)nullptr,
        (IBatAttackVftable::OnAttack)nullptr,
    },
}};

static std::array<IBatAttackVftable*, 4> vftables = {{
    // Akella
    (IBatAttackVftable*)0x6f4ffc,
    // Russobit
    (IBatAttackVftable*)0x6f4ffc,
    // Gog
    (IBatAttackVftable*)0x6f2fac,
    // Scenario Editor
    (IBatAttackVftable*)nullptr,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

IBatAttackVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CBatAttackBestowWardsApi
