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

#include "midgardid.h"
#include "version.h"
#include <array>

namespace game {

const CMidgardID invalidId{0x3f0000};
const CMidgardID emptyId{0};

namespace CMidgardIDApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::GetCategory)0x57fcc5,
        (Api::GetCategoryIndex)0x57fcde,
        (Api::GetType)0x57fcfe,
        (Api::GetTypeIndex)0x57fd1a,
        (Api::ToString)0x57fd37,
        (Api::FromString)0x57f897,
        (Api::FromParts)0x57fbfa,
        (Api::IsIdStringValid)0x57fb49
    },
    // Russobit
    Api{
        (Api::GetCategory)0x57fcc5,
        (Api::GetCategoryIndex)0x57fcde,
        (Api::GetType)0x57fcfe,
        (Api::GetTypeIndex)0x57fd1a,
        (Api::ToString)0x57fd37,
        (Api::FromString)0x57f897,
        (Api::FromParts)0x57fbfa,
        (Api::IsIdStringValid)0x57fb49
    },
    // Gog
    Api{
        (Api::GetCategory)0x57f37d,
        (Api::GetCategoryIndex)0x57f396,
        (Api::GetType)0x57f3b6,
        (Api::GetTypeIndex)0x57f3d2,
        (Api::ToString)0x57f3ef,
        (Api::FromString)0x57ef4f,
        (Api::FromParts)0x57f2b2,
        (Api::IsIdStringValid)0x57f201
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace CMidgardIDApi

} // namespace game
