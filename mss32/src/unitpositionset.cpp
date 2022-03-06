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

#include "unitpositionset.h"
#include "version.h"
#include <array>

namespace game::UnitPositionSetApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x640100,
        (Api::Destructor)0x6401a0,
        (Api::CopyConstructor)0x640180,
        (Api::CopyAssignment)0x6401c0,
        (Api::GetIterator)0x6403f0,
        (Api::GetIterator)0x6401e0,
        (Api::Dereference)0x62b790,
        (Api::Equals)0x6402e0,
        (Api::Preincrement)0x640560,
        (Api::FindByPosition)0x634eaf,
        (Api::HasNegativePosition)0x631bfb,
    },
    // Russobit
    Api{
        (Api::Constructor)0x640100,
        (Api::Destructor)0x6401a0,
        (Api::CopyConstructor)0x640180,
        (Api::CopyAssignment)0x6401c0,
        (Api::GetIterator)0x6403f0,
        (Api::GetIterator)0x6401e0,
        (Api::Dereference)0x62b790,
        (Api::Equals)0x6402e0,
        (Api::Preincrement)0x640560,
        (Api::FindByPosition)0x634eaf,
        (Api::HasNegativePosition)0x631bfb,
    },
    // Gog
    Api{
        (Api::Constructor)0x63eb00,
        (Api::Destructor)0x63eba0,
        (Api::CopyConstructor)0x63eb80,
        (Api::CopyAssignment)0x63ebc0,
        (Api::GetIterator)0x667400,
        (Api::GetIterator)0x63ebe0,
        (Api::Dereference)0x63eeb0,
        (Api::Equals)0x6678d0,
        (Api::Preincrement)0x642390,
        (Api::FindByPosition)0x6338ef,
        (Api::HasNegativePosition)0x63063b,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::UnitPositionSetApi
