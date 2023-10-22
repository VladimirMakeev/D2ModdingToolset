/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#include "displayd3d.h"
#include "version.h"
#include <array>

namespace game::CDisplayD3DApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::DrawPrimitive)0x6776d0,
        (Api::SwapBuffers)0x676a40,
        (Api::AlphaBlend)0x6775f0,
    },
    // Russobit
    Api{
        (Api::DrawPrimitive)0x6776d0,
        (Api::SwapBuffers)0x676a40,
        (Api::AlphaBlend)0x6775f0,
    },
    // Gog
    Api{
        (Api::DrawPrimitive)0x675fb0,
        (Api::SwapBuffers)0x675320,
        (Api::AlphaBlend)0x675ed0,
    },
    // Scenario Editor
    Api{
        (Api::DrawPrimitive)0x58eaa0,
        (Api::SwapBuffers)0x58de10,
        (Api::AlphaBlend)0x58e9c0,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CDisplayD3DApi
