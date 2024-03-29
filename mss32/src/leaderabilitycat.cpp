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

#include "leaderabilitycat.h"
#include "version.h"
#include <array>

namespace game::LeaderAbilityCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LLeaderAbility*)0x8395e8,
        (LLeaderAbility*)0x8395f8,
        (LLeaderAbility*)0x839608,
        (LLeaderAbility*)0x839618,
        (LLeaderAbility*)0x839628,
        (LLeaderAbility*)0x839638,
        (LLeaderAbility*)0x839648,
        (LLeaderAbility*)0x839658,
        (LLeaderAbility*)0x839668,
        (LLeaderAbility*)0x839678,
        (LLeaderAbility*)0x839688,
    },
    // Russobit
    Categories{
        (LLeaderAbility*)0x8395e8,
        (LLeaderAbility*)0x8395f8,
        (LLeaderAbility*)0x839608,
        (LLeaderAbility*)0x839618,
        (LLeaderAbility*)0x839628,
        (LLeaderAbility*)0x839638,
        (LLeaderAbility*)0x839648,
        (LLeaderAbility*)0x839658,
        (LLeaderAbility*)0x839668,
        (LLeaderAbility*)0x839678,
        (LLeaderAbility*)0x839688,
    },
    // Gog
    Categories{
        (LLeaderAbility*)0x837598,
        (LLeaderAbility*)0x8375a8,
        (LLeaderAbility*)0x8375b8,
        (LLeaderAbility*)0x8375c8,
        (LLeaderAbility*)0x8375d8,
        (LLeaderAbility*)0x8375e8,
        (LLeaderAbility*)0x8375f8,
        (LLeaderAbility*)0x837608,
        (LLeaderAbility*)0x837618,
        (LLeaderAbility*)0x837628,
        (LLeaderAbility*)0x837638,
    },
    // Scenario Editor
    Categories{
        (LLeaderAbility*)0x665840,
        (LLeaderAbility*)0x665850,
        (LLeaderAbility*)0x665860,
        (LLeaderAbility*)0x665870,
        (LLeaderAbility*)0x665880,
        (LLeaderAbility*)0x665890,
        (LLeaderAbility*)0x6658A0,
        (LLeaderAbility*)0x6658B0,
        (LLeaderAbility*)0x6658C0,
        (LLeaderAbility*)0x6658D0,
        (LLeaderAbility*)0x6658E0,
    }
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::LeaderAbilityCategories
