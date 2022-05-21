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

#include "umattack.h"
#include "version.h"
#include <array>

namespace game::CUmAttackApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x5a56b9,
        (Api::CopyConstructor)0x5a5939,
        (Api::DataConstructor)0x5a58a7,
        (Api::DataCopyConstructor)0x5a59ce,
        (Api::ReadData)0x5a57cf,
    },
    // Russobit
    Api{
        (Api::Constructor)0x5a56b9,
        (Api::CopyConstructor)0x5a5939,
        (Api::DataConstructor)0x5a58a7,
        (Api::DataCopyConstructor)0x5a59ce,
        (Api::ReadData)0x5a57cf,
    },
    // Gog
    Api{
        (Api::Constructor)0x5a4949,
        (Api::CopyConstructor)0x5a4bc9,
        (Api::DataConstructor)0x5a4b37,
        (Api::DataCopyConstructor)0x5a4c5e,
        (Api::ReadData)0x5a4a5f,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x54d852,
        (Api::CopyConstructor)0x54dad2,
        (Api::DataConstructor)0x54da40,
        (Api::DataCopyConstructor)0x54db67,
        (Api::ReadData)0x54d968,
    }
}};

static std::array<IUsSoldierVftable*, 4> vftables = {{
    // Akella
    (IUsSoldierVftable*)0x6ecf2c,
    // Russobit
    (IUsSoldierVftable*)0x6ecf2c,
    // Gog
    (IUsSoldierVftable*)0x6eaecc,
    // Scenario Editor
    (IUsSoldierVftable*)0x5e10ec,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

const IUsSoldierVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CUmAttackApi
