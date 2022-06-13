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

#include "imageptrvector.h"
#include "version.h"
#include <array>

namespace game::ImagePtrVectorApi {

// clang-format off
static std::array<Api, 4> functions = { {
    // Akella
    Api{
        (Api::Destructor)0x495373,
        (Api::Reserve)0x4957e2,
        (Api::PushBack)0x4a4fd1,
    },
    // Russobit
    Api{
        (Api::Destructor)0x495373,
        (Api::Reserve)0x4957e2,
        (Api::PushBack)0x4a4fd1,
    },
    // Gog
    Api{
        (Api::Destructor)0x494df0,
        (Api::Reserve)0x495293,
        (Api::PushBack)0x4a4852,
    },
    // Scenario Editor
    Api{
        (Api::Destructor)0x4c871d,
        (Api::Reserve)0x4c8a7f,
        (Api::PushBack)0x4c8a01,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::ImagePtrVectorApi
