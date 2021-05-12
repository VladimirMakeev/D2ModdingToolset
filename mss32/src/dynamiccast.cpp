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

#include "dynamiccast.h"
#include "version.h"
#include <array>

namespace game::RttiApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api {
        (Api::DynamicCast)0x66d466
    },
    // Russobit
    Api {
        (Api::DynamicCast)0x66d466
    },
    // Gog
    Api {
        (Api::DynamicCast)0x66bda6
    }
}};

static const std::array<Rtti, 3> types = {{
    // Akella
    Rtti{
        (TypeDescriptor*)0x78eda8,
        (TypeDescriptor*)0x78edd0,
        (TypeDescriptor*)0x79e868,
        (TypeDescriptor*)0x79e600,
        (TypeDescriptor*)0x79e620,
        (TypeDescriptor*)0x78ef80,
        (TypeDescriptor*)0x7b00e8,
        (TypeDescriptor*)0x7b0120,
        (TypeDescriptor*)0x78ee30,
        (TypeDescriptor*)0x78eee0,
        (TypeDescriptor*)0x7962b8,
        (TypeDescriptor*)0x78fd70,
        (TypeDescriptor*)0x7927b8,
        (TypeDescriptor*)0x7927e0,
        (TypeDescriptor*)0x792808,
        (TypeDescriptor*)0x78fd98,
        (TypeDescriptor*)0x793600,
        (TypeDescriptor*)0x793630,
        (TypeDescriptor*)0x792828,
    },
    // Russobit
    Rtti{
        (TypeDescriptor*)0x78eda8,
        (TypeDescriptor*)0x78edd0,
        (TypeDescriptor*)0x79e868,
        (TypeDescriptor*)0x79e600,
        (TypeDescriptor*)0x79e620,
        (TypeDescriptor*)0x78ef80,
        (TypeDescriptor*)0x7b00e8,
        (TypeDescriptor*)0x7b0120,
        (TypeDescriptor*)0x78ee30,
        (TypeDescriptor*)0x78eee0,
        (TypeDescriptor*)0x7962b8,
        (TypeDescriptor*)0x78fd70,
        (TypeDescriptor*)0x7927b8,
        (TypeDescriptor*)0x7927e0,
        (TypeDescriptor*)0x792808,
        (TypeDescriptor*)0x78fd98,
        (TypeDescriptor*)0x793600,
        (TypeDescriptor*)0x793630,
        (TypeDescriptor*)0x792828,
    },
    // Gog
    Rtti{
        (TypeDescriptor*)0x78cd50,
        (TypeDescriptor*)0x78cd78,
        (TypeDescriptor*)0x79c810,
        (TypeDescriptor*)0x79c5a8,
        (TypeDescriptor*)0x79c5c8,
        (TypeDescriptor*)0x78cf28,
        (TypeDescriptor*)0x7ae0a0,
        (TypeDescriptor*)0x7ae0d8,
        (TypeDescriptor*)0x78cdd8,
        (TypeDescriptor*)0x78ce88,
        (TypeDescriptor*)0x794260,
        (TypeDescriptor*)0x78dd18,
        (TypeDescriptor*)0x790760,
        (TypeDescriptor*)0x790788,
        (TypeDescriptor*)0x7907b0,
        (TypeDescriptor*)0x78dd40,
        (TypeDescriptor*)0x7915a8,
        (TypeDescriptor*)0x7915d8,
        (TypeDescriptor*)0x7907d0,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

const Rtti& rtti()
{
    return types[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::RttiApi
