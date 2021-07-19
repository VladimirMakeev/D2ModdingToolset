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

#include "unitinfolist.h"
#include "version.h"
#include <array>

namespace game::UnitInfoListApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x41bf2a,
        (Api::Destructor)0x41bf63,
        (Api::Sort)0x640600,
        (Api::GetIterator)0x4dee1a,
        (Api::GetIterator)0x625030,
        (Api::Dereference)0x62d280,
        (Api::Equals)0x474a74,
        (Api::Preincrement)0x62b7f0,
    },
    // Russobit
    Api{
        (Api::Constructor)0x41bf2a,
        (Api::Destructor)0x41bf63,
        (Api::Sort)0x640600,
        (Api::GetIterator)0x4dee1a,
        (Api::GetIterator)0x625030,
        (Api::Dereference)0x62d280,
        (Api::Equals)0x474a74,
        (Api::Preincrement)0x62b7f0,
    },
    // Gog
    Api{
        (Api::Constructor)0x41b9f7,
        (Api::Destructor)0x41ba30,
        (Api::Sort)0x63eed0,
        (Api::GetIterator)0x4c7e28,
        (Api::GetIterator)0x623b90,
        (Api::Dereference)0x62c8e0,
        (Api::Equals)0x5d8fbb,
        (Api::Preincrement)0x62a310,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::UnitInfoListApi
