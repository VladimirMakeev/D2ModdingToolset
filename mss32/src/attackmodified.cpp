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

#include "attackmodified.h"
#include "version.h"
#include <array>

namespace game::CAttackModifiedApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x5aa0ca,
        (Api::CopyConstructor)0x5aa1cb,
        (Api::Wrap)0x5aa317,
    },
    // Russobit
    Api{
        (Api::Constructor)0x5aa0ca,
        (Api::CopyConstructor)0x5aa1cb,
        (Api::Wrap)0x5aa317,
    },
    // Gog
    Api{
        (Api::Constructor)0x5a9352,
        (Api::CopyConstructor)0x5a9453,
        (Api::Wrap)0x5a959f,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x55142b,
        (Api::CopyConstructor)0x55152c,
        (Api::Wrap)0x551678,
    },
}};

static std::array<IAttackVftable*, 4> vftables = {{
    // Akella
    (IAttackVftable*)0x6ed69c,
    // Russobit
    (IAttackVftable*)0x6ed69c,
    // Gog
    (IAttackVftable*)0x6eb63c,
    // Scenario Editor
    (IAttackVftable*)0x5e17e4,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

const IAttackVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CAttackModifiedApi
