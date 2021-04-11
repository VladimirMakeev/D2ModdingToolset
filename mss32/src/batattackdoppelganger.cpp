/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#include "batattackdoppelganger.h"
#include "version.h"
#include <array>

namespace game::CBatAttackDoppelgangerApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (IBatAttackVftable::CanPerform)0x6633f3,
        (IBatAttackVftable::IsImmune)0x66360b,
        (IBatAttackVftable::OnAttack)0x66374d,
    },
    // Russobit
    Api{
        (IBatAttackVftable::CanPerform)0x6633f3,
        (IBatAttackVftable::IsImmune)0x66360b,
        (IBatAttackVftable::OnAttack)0x66374d,
    },
    // Gog
    Api{
        (IBatAttackVftable::CanPerform)0x661e73,
        (IBatAttackVftable::IsImmune)0x66208b,
        (IBatAttackVftable::OnAttack)0x6621cd,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CBatAttackDoppelgangerApi
