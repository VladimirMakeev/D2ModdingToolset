/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#include "midunitdescriptor.h"
#include "version.h"
#include <array>

namespace game::CMidUnitDescriptorApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x57e871,
        (Api::GetSoldier)0x57eda2,
        (Api::GetAttack)0x57edb8,
        (Api::GetAltAttack)0x57ee00,
    },
    // Russobit
    Api{
        (Api::Constructor)0x57e871,
        (Api::GetSoldier)0x57eda2,
        (Api::GetAttack)0x57edb8,
        (Api::GetAltAttack)0x57ee00,
    },
    // Gog
    Api{
        (Api::Constructor)0x57df2c,
        (Api::GetSoldier)0x57e45d,
        (Api::GetAttack)0x57e473,
        (Api::GetAltAttack)0x57e4bb,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x4cfcb5,
        (Api::GetSoldier)0x4d01e6,
        (Api::GetAttack)0x4d01fc,
        (Api::GetAltAttack)0x4d0244,
    },
}};

static std::array<IEncUnitDescriptorVftable*, 4> vftables = {{
    // Akella
    (IEncUnitDescriptorVftable*)0x6e919c,
    // Russobit
    (IEncUnitDescriptorVftable*)0x6e919c,
    // Gog
    (IEncUnitDescriptorVftable*)0x6e713c,
    // Scenario Editor
    (IEncUnitDescriptorVftable*)0x5d869c,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

const IEncUnitDescriptorVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMidUnitDescriptorApi
