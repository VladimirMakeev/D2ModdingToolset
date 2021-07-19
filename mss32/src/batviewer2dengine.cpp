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

#include "batviewer2dengine.h"
#include "version.h"
#include <array>

namespace game::BatViewer2DEngineApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::ClearTargetMarks)0x6483e9,
        (Api::DrawTargetMark)0x648394,
    },
    // Russobit
    Api{
        (Api::ClearTargetMarks)0x6483e9,
        (Api::DrawTargetMark)0x648394,
    },
    // Gog
    Api{
        (Api::ClearTargetMarks)0x646c69,
        (Api::DrawTargetMark)0x646c14,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::BatViewer2DEngineApi
