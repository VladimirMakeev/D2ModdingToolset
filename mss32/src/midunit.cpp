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
        (Api::Upgrade)0x5eb461,
        (Api::Transform)0x5eb559,
        (Api::Untransform)0x5eb6ba,
        (Api::GetModifiers)0x5eabc0,
        (Api::AddModifiers)0x5eb1df,
        (Api::RemoveModifiers)0x5ea9ea,
        (Api::ReplaceImpl)0x5eb4fc,
        (Api::StreamImpl)0x5eaf8d,
        (Api::StreamModifiers)0x5eb010,
    },
    // Russobit
    Api{
        (Api::AddRemoveModifier)0x5eabff,
        (Api::AddRemoveModifier)0x5eac81,
        (Api::GetHpMax)0x5eab79,
        (Api::Upgrade)0x5eb461,
        (Api::Transform)0x5eb559,
        (Api::Untransform)0x5eb6ba,
        (Api::GetModifiers)0x5eabc0,
        (Api::AddModifiers)0x5eb1df,
        (Api::RemoveModifiers)0x5ea9ea,
        (Api::ReplaceImpl)0x5eb4fc,
        (Api::StreamImpl)0x5eaf8d,
        (Api::StreamModifiers)0x5eb010,
    },
    // Gog
    Api{
        (Api::AddRemoveModifier)0x5e9902,
        (Api::AddRemoveModifier)0x5e9984,
        (Api::GetHpMax)0x5e987c,
        (Api::Upgrade)0x5ea164,
        (Api::Transform)0x5ea25c,
        (Api::Untransform)0x5ea3bd,
        (Api::GetModifiers)0x5e98c3,
        (Api::AddModifiers)0x5e9ee2,
        (Api::RemoveModifiers)0x5e96e9,
        (Api::ReplaceImpl)0x5ea1ff,
        (Api::StreamImpl)0x5e9c90,
        (Api::StreamModifiers)0x5e9d13,
    },
    // Scenario Editor
    Api{
        (Api::AddRemoveModifier)0x4ed994,
        (Api::AddRemoveModifier)0x4eda16,
        (Api::GetHpMax)0x4ed90e,
        (Api::Upgrade)nullptr,
        (Api::Transform)nullptr,
        (Api::Untransform)nullptr,
        (Api::GetModifiers)0x4ed955,
        (Api::AddModifiers)0x4edf15,
        (Api::RemoveModifiers)0x4ed77b,
        (Api::ReplaceImpl)nullptr,
        (Api::StreamImpl)0x4edcc3,
        (Api::StreamModifiers)0x4edd46,
    },
}};

static std::array<CMidUnitVftable*, 4> vftables = {{
    // Akella
    (CMidUnitVftable*)0x6f0914,
    // Russobit
    (CMidUnitVftable*)0x6f0914,
    // Gog
    (CMidUnitVftable*)0x6ee8b4,
    // Scenario Editor
    (CMidUnitVftable*)0x5da34c,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

CMidUnitVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMidUnitApi
