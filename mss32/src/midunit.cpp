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

#include "midunit.h"
#include "version.h"
#include <array>

namespace game::CMidUnitApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::AddRemoveModifier)0x5eabff,
        (Api::AddRemoveModifier)0x5eac81,
        (Api::GetHpMax)0x5eab79,
        (Api::Transform)0x5eb559,
        (Api::Untransform)0x5eb6ba,
    },
    // Russobit
    Api{
        (Api::AddRemoveModifier)0x5eabff,
        (Api::AddRemoveModifier)0x5eac81,
        (Api::GetHpMax)0x5eab79,
        (Api::Transform)0x5eb559,
        (Api::Untransform)0x5eb6ba,
    },
    // Gog
    Api{
        (Api::AddRemoveModifier)0x5e9902,
        (Api::AddRemoveModifier)0x5e9984,
        (Api::GetHpMax)0x5e987c,
        (Api::Transform)0x5ea25c,
        (Api::Untransform)0x5ea3bd,
    },
    // Scenario Editor
    Api{
        (Api::AddRemoveModifier)0x4ed994,
        (Api::AddRemoveModifier)0x4eda16,
        (Api::GetHpMax)0x4ed90e,
        (Api::Transform)nullptr,
        (Api::Untransform)nullptr,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMidUnitApi
