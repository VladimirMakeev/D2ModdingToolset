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

#include "midgardscenariomap.h"
#include "version.h"
#include <array>

namespace game::CMidgardScenarioMapApi {

// clang-format off
std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::GetIterator)0x5ef64a,
        (Api::GetIterator)0x5ef664,
        (Api::Advance)0x5f0222,
        (Api::CheckObjects)0x5efcd1,
        (Api::Stream)0x5efd6c,
    },
    // Russobit
    Api{
        (Api::GetIterator)0x5ef64a,
        (Api::GetIterator)0x5ef664,
        (Api::Advance)0x5f0222,
        (Api::CheckObjects)0x5efcd1,
        (Api::Stream)0x5efd6c,
    },
    // Gog
    Api{
        (Api::GetIterator)0x5ee30f,
        (Api::GetIterator)0x5ee329,
        (Api::Advance)0x410884,
        (Api::CheckObjects)0x5ee996,
        (Api::Stream)0x5eea31,
    },
    // Scenario Editor
    Api{
        (Api::GetIterator)0x4dbde5,
        (Api::GetIterator)0x4dbdff,
        (Api::Advance)0x4dc693,
        (Api::CheckObjects)0x4dc157,
        (Api::Stream)0x4dc1f2,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMidgardScenarioMapApi
