/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#include "managestkinterf.h"
#include "version.h"
#include <array>

namespace game::CManageStkInterfApi {

// clang-format off
static std::array<Vftable, 4> vftables = {{
    // Akella
    Vftable{
        (CMidDataCache2::INotifyVftable*)0x6d751c,
        (IResetStackExtVftable*)0x6d74fc,
        (CInterfaceVftable*)0x6d746c,
        (ITaskManagerHolderVftable*)0x6d745c,
        (IMidDropManagerVftable*)0x6d741c,
    },
    // Russobit
    Vftable{
        (CMidDataCache2::INotifyVftable*)0x6d751c,
        (IResetStackExtVftable*)0x6d74fc,
        (CInterfaceVftable*)0x6d746c,
        (ITaskManagerHolderVftable*)0x6d745c,
        (IMidDropManagerVftable*)0x6d741c,
    },
    // Gog
    Vftable{
        (CMidDataCache2::INotifyVftable*)0x6d54bc,
        (IResetStackExtVftable*)0x6d549c,
        (CInterfaceVftable*)0x6d540c,
        (ITaskManagerHolderVftable*)0x6d53fc,
        (IMidDropManagerVftable*)0x6d53bc,
    },
    // Scenario Editor
    Vftable{},
}};
// clang-format on

Vftable& vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CManageStkInterfApi
