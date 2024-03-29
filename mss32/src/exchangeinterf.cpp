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

#include "exchangeinterf.h"
#include "version.h"
#include <array>

namespace game::CExchangeInterfApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x493296,
        (Api::CreateButtonFunctor)0x495468
    },
    // Russobit
    Api{
        (Api::Constructor)0x493296,
        (Api::CreateButtonFunctor)0x495468
    },
    // Gog
    Api{
        (Api::Constructor)0x492cee,
        (Api::CreateButtonFunctor)0x494ebb
    }
}};

static std::array<Vftable, 4> vftables = {{
    // Akella
    Vftable{
        (CMidDataCache2::INotifyVftable*)0x6d70cc,
        (CInterfaceVftable*)0x6d703c,
        (ITaskManagerHolderVftable*)0x6d702c,
        (IMidDropManagerVftable*)0x6d6fec,
    },
    // Russobit
    Vftable{
        (CMidDataCache2::INotifyVftable*)0x6d70cc,
        (CInterfaceVftable*)0x6d703c,
        (ITaskManagerHolderVftable*)0x6d702c,
        (IMidDropManagerVftable*)0x6d6fec,
    },
    // Gog
    Vftable{
        (CMidDataCache2::INotifyVftable*)0x6d506c,
        (CInterfaceVftable*)0x6d4fdc,
        (ITaskManagerHolderVftable*)0x6d4fcc,
        (IMidDropManagerVftable*)0x6d4f8c,
    },
    // Scenario Editor
    Vftable{},
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

} // namespace game::CExchangeInterfApi
