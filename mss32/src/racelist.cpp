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

#include "racelist.h"
#include "version.h"
#include <array>

namespace game::RaceCategoryListApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x424628,
        (Api::CopyConstructor)0x42c485,
        (Api::FreeNodes)0x40a549,
        (Api::FreeNode)0x4c8755,
        (Api::Add)0x42c546,
        (Api::GetPlayableRaces)0x4ec2a4,
    },
    // Russobit
    Api{
        (Api::Constructor)0x424628,
        (Api::CopyConstructor)0x42c485,
        (Api::FreeNodes)0x40a549,
        (Api::FreeNode)0x4c8755,
        (Api::Add)0x42c546,
        (Api::GetPlayableRaces)0x4ec2a4,
    },
    // Gog
    Api{
        (Api::Constructor)0x42413b,
        (Api::CopyConstructor)0x42bea5,
        (Api::FreeNodes)0x40a22b,
        (Api::FreeNode)0x5215fc,
        (Api::Add)0x42bf66,
        (Api::GetPlayableRaces)0x4eb764,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::RaceCategoryListApi
