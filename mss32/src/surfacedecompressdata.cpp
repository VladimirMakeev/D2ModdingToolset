/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#include "surfacedecompressdata.h"
#include "version.h"
#include <array>

namespace game::SurfaceDecompressDataApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::ConvertColor)0x5619e2,
        (Api::FillArea)0x5616be,
        (Api::SetColor)0x561694,
        (Api::DrawTextString)0x534d0b,
    },
    // Russobit
    Api{
        (Api::ConvertColor)0x5619e2,
        (Api::FillArea)0x5616be,
        (Api::SetColor)0x561694,
        (Api::DrawTextString)0x534d0b,
    },
    // Gog
    Api{
        (Api::ConvertColor)0x56117f,
        (Api::FillArea)0x560e5b,
        (Api::SetColor)0x560e31,
        (Api::DrawTextString)0x5342e9,
    },
    // Scenario Editor
    Api{
        (Api::ConvertColor)0x584553,
        (Api::FillArea)0x58422f,
        (Api::SetColor)0x584205,
        (Api::DrawTextString)0x48ea22,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::SurfaceDecompressDataApi
