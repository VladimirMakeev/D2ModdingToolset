/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#include "targetslist.h"
#include "version.h"
#include <array>

namespace game::TargetsListApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x42904e,
        (Api::Destructor)0x416331,
        (Api::Fill)0x65b725,
        (Api::Clear)0x668710,
        (Api::Begin)0x6686d0,
        (Api::Insert)0x477d8a,
        (Api::Remove)0x6686f0,
        (Api::Get)0x62b790
    },
    // Russobit
    Api{
        (Api::Constructor)0x42904e,
        (Api::Destructor)0x416331,
        (Api::Fill)0x65b725,
        (Api::Clear)0x668710,
        (Api::Begin)0x6686d0,
        (Api::Insert)0x477d8a,
        (Api::Remove)0x6686f0,
        (Api::Get)0x62b790
    },
    // Gog
    Api{
        (Api::Constructor)0x428ada,
        (Api::Destructor)0x415fdc,
        (Api::Fill)0x65a1a5,
        (Api::Clear)0x667190,
        (Api::Begin)0x667150,
        (Api::Insert)0x477985,
        (Api::Remove)0x667170,
        (Api::Get)0x63eeb0
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::TargetsListApi
