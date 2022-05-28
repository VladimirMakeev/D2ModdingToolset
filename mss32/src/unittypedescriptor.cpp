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

#include "unittypedescriptor.h"
#include "version.h"
#include <array>

namespace game::CUnitTypeDescriptorApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::GetUnitImpl)0x57f2ac,
        (Api::GetAttack)0x57f2c4,
        (Api::GetAltAttack)0x57f312,
    },
    // Russobit
    Api{
        (Api::GetUnitImpl)0x57f2ac,
        (Api::GetAttack)0x57f2c4,
        (Api::GetAltAttack)0x57f312,
    },
    // Gog
    Api{
        (Api::GetUnitImpl)0x57e964,
        (Api::GetAttack)0x57e97c,
        (Api::GetAltAttack)0x57e9ca,
    },
    // Scenario Editor
    Api{
        (Api::GetUnitImpl)0x4d070f,
        (Api::GetAttack)0x4d0727,
        (Api::GetAltAttack)0x4d0775,
    },
}};

static std::array<IEncUnitDescriptorVftable*, 4> vftables = {{
    // Akella
    (IEncUnitDescriptorVftable*)0x6e90ec,
    // Russobit
    (IEncUnitDescriptorVftable*)0x6e90ec,
    // Gog
    (IEncUnitDescriptorVftable*)0x6e708c,
    // Scenario Editor
    (IEncUnitDescriptorVftable*)0x5d853c,
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

} // namespace game::CUnitTypeDescriptorApi
