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

#include "visitors.h"
#include "version.h"
#include <array>

namespace game::VisitorApi {

// clang-format off
std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::ChangeUnitHp)0x5e88f4,
        (Api::ChangeUnitXp)0x5e8942,
        (Api::UpgradeUnit)0x5e963d,
        (Api::ForceUnitMax)0x5e972a,
        (Api::AddUnitToGroup)0x5e8bf8,
        (Api::ExchangeItem)0x5e86cc,
        (Api::TransformUnit)0x5e968e,
        (Api::UndoTransformUnit)0x5e96df,
        (Api::ExtractUnitFromGroup)0x5e8d72,
        (Api::PlayerSetAttitude)0,
    },
    // Russobit
    Api{
        (Api::ChangeUnitHp)0x5e88f4,
        (Api::ChangeUnitXp)0x5e8942,
        (Api::UpgradeUnit)0x5e963d,
        (Api::ForceUnitMax)0x5e972a,
        (Api::AddUnitToGroup)0x5e8bf8,
        (Api::ExchangeItem)0x5e86cc,
        (Api::TransformUnit)0x5e968e,
        (Api::UndoTransformUnit)0x5e96df,
        (Api::ExtractUnitFromGroup)0x5e8d72,
        (Api::PlayerSetAttitude)0,
    },
    // Gog
    Api{
        (Api::ChangeUnitHp)0x5e75f3,
        (Api::ChangeUnitXp)0x5e7641,
        (Api::UpgradeUnit)0x5e833c,
        (Api::ForceUnitMax)0x5e8429,
        (Api::AddUnitToGroup)0x5e78f7,
        (Api::ExchangeItem)0x5e73cb,
        (Api::TransformUnit)0x5e838d,
        (Api::UndoTransformUnit)0x5e83de,
        (Api::ExtractUnitFromGroup)0x5e7a71,
        (Api::PlayerSetAttitude)0,
    },
    // Scenario Editor
    Api{
        (Api::ChangeUnitHp)0,
        (Api::ChangeUnitXp)0,
        (Api::UpgradeUnit)0,
        (Api::ForceUnitMax)0,
        (Api::AddUnitToGroup)0,
        (Api::ExchangeItem)0,
        (Api::TransformUnit)0,
        (Api::UndoTransformUnit)0,
        (Api::ExtractUnitFromGroup)0,
        (Api::PlayerSetAttitude)0x4e9baa,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::VisitorApi
