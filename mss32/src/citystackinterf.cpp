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

#include "citystackinterf.h"
#include "version.h"
#include <array>

namespace game::CCityStackInterfApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x4b14cd,
        (Api::CreateButtonFunctor)0x4b4bca
    },
    // Russobit
    Api{
        (Api::Constructor)0x4b14cd,
        (Api::CreateButtonFunctor)0x4b4bca
    },
    // Gog
    Api{
        (Api::Constructor)0x4b0bcf,
        (Api::CreateButtonFunctor)0x4b4263
    }
}};

static std::array<Vftable, 4> vftables = {{
    // Akella
    Vftable{
        (CMidDataCache2::INotifyVftable*)0x6d96e4,
        (CMidCommandQueue2::INotifyCQVftable*)0x6d96d4,
        (IResetStackExtVftable*)0x6d96b4,
        (CInterfaceVftable*)0x6d9624,
        (ITaskManagerHolderVftable*)0x6d9614,
        (IMidDropManagerVftable*)0x6d95d4,
    },
    // Russobit
    Vftable{
        (CMidDataCache2::INotifyVftable*)0x6d96e4,
        (CMidCommandQueue2::INotifyCQVftable*)0x6d96d4,
        (IResetStackExtVftable*)0x6d96b4,
        (CInterfaceVftable*)0x6d9624,
        (ITaskManagerHolderVftable*)0x6d9614,
        (IMidDropManagerVftable*)0x6d95d4,
    },
    // Gog
    Vftable{
        (CMidDataCache2::INotifyVftable*)0x6d7684,
        (CMidCommandQueue2::INotifyCQVftable*)0x6d7674,
        (IResetStackExtVftable*)0x6d7654,
        (CInterfaceVftable*)0x6d75c4,
        (ITaskManagerHolderVftable*)0x6d75b4,
        (IMidDropManagerVftable*)0x6d7574,
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

} // namespace game::CCityStackInterfApi
