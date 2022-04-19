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

#include "mainview2.h"
#include "version.h"
#include <array>

namespace game::CMainView2Api {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::ShowIsoDialog)0x4893a5,
        (Api::ShowDialog)0x4889d8,
        (Api::CreateToggleButtonFunctor)0x48c5d7,
    },
    // Russobit
    Api{
        (Api::ShowIsoDialog)0x4893a5,
        (Api::ShowDialog)0x4889d8,
        (Api::CreateToggleButtonFunctor)0x48c5d7,
    },
    // Gog
    Api{
        (Api::ShowIsoDialog)0x488f8e,    
        (Api::ShowDialog)0x4885c1,
        (Api::CreateToggleButtonFunctor)0x48c19d,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMainView2Api
