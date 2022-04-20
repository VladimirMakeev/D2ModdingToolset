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

#include "umunit.h"
#include "version.h"
#include <array>

namespace game::CUmUnitApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x5a49b0,
        (Api::CopyConstructor)0x5a4d0a,
        (Api::DataConstructor)0x5a4c3a,
        (Api::DataCopyConstructor)0x5a4d9f,
        (Api::ReadData)0x5a4a8a,
    },
    // Russobit
    Api{
        (Api::Constructor)0x5a49b0,
        (Api::CopyConstructor)0x5a4d0a,
        (Api::DataConstructor)0x5a4c3a,
        (Api::DataCopyConstructor)0x5a4d9f,
        (Api::ReadData)0x5a4a8a,
    },
    // Gog
    Api{
        (Api::Constructor)0x5a3c39,
        (Api::CopyConstructor)0x5a3f93,
        (Api::DataConstructor)0x5a3ec3,
        (Api::DataCopyConstructor)0x5a4028,
        (Api::ReadData)0x5a3d13,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x54cc1b,
        (Api::CopyConstructor)0x54cf75,
        (Api::DataConstructor)0x54cea5,
        (Api::DataCopyConstructor)0x54d00a,
        (Api::ReadData)0x54ccf5,
    },
}};

static std::array<Vftable, 4> vftables = {{
    // Akella
    Vftable{
        (IUsUnitVftable*)0x6ece74,
        (IUsSoldierVftable*)0x6ecdfc,
        (CUmModifierVftable*)0x6ecdcc,
    },
    // Russobit
    Vftable{
        (IUsUnitVftable*)0x6ece74,
        (IUsSoldierVftable*)0x6ecdfc,
        (CUmModifierVftable*)0x6ecdcc,
    },
    // Gog
    Vftable{
        (IUsUnitVftable*)0x6eae14,
        (IUsSoldierVftable*)0x6ead9c,
        (CUmModifierVftable*)0x6ead6c,
    },
    // Scenario Editor
    Vftable{
        (IUsUnitVftable*)0x5e1034,
        (IUsSoldierVftable*)0x5e0fbc,
        (CUmModifierVftable*)0x5e0f8c,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

Vftable& vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CUmUnitApi
