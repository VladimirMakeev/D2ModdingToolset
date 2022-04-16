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

#include "batattackdrainoverflow.h"
#include "version.h"
#include <array>

namespace game::CBatAttackDrainOverflowApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::HealDataConstructor)0x668910,
        (Api::HealDataDestructor)0x668990,
        (Api::HealDataIteratorConstructor)0x6403f0,
        (Api::HealDataEndIteratorConstructor)0x6689b0,
        (Api::HealDataIteratorCopyConstructor)0x651af0,
        (Api::IsHealDataIteratorAtEnd)0x668c60,
        (Api::HealDataIteratorAdvance)0x668c20,
        (Api::HealDataIteratorGetData)0x62b790,
        (Api::ComputeDrainOverflowGroupHeal)0x661138
    },
    // Russobit
    Api{
        (Api::HealDataConstructor)0x668910,
        (Api::HealDataDestructor)0x668990,
        (Api::HealDataIteratorConstructor)0x6403f0,
        (Api::HealDataEndIteratorConstructor)0x6689b0,
        (Api::HealDataIteratorCopyConstructor)0x651af0,
        (Api::IsHealDataIteratorAtEnd)0x668c60,
        (Api::HealDataIteratorAdvance)0x668c20,
        (Api::HealDataIteratorGetData)0x62b790,
        (Api::ComputeDrainOverflowGroupHeal)0x661138
    },
    // Gog
    Api{
        (Api::HealDataConstructor)0x667360,
        (Api::HealDataDestructor)0x6673e0,
        (Api::HealDataIteratorConstructor)0x667400,
        (Api::HealDataEndIteratorConstructor)0x667420,
        (Api::HealDataIteratorCopyConstructor)0x63f650,
        (Api::IsHealDataIteratorAtEnd)0x6676d0,
        (Api::HealDataIteratorAdvance)0x667690,
        (Api::HealDataIteratorGetData)0x63eeb0,
        (Api::ComputeDrainOverflowGroupHeal)0x65fbb8
    }
}};

static std::array<IBatAttackVftable*, 4> vftables = {{
    // Akella
    (IBatAttackVftable*)0x6f536c,
    // Russobit
    (IBatAttackVftable*)0x6f536c,
    // Gog
    (IBatAttackVftable*)0x6f331c,
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

} // namespace game::CBatAttackDrainOverflowApi
