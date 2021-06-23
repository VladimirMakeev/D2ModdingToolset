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

#include "battleviewerinterf.h"
#include "version.h"
#include <array>

namespace game::BattleViewerInterfApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::MarkAttackTargets)0x6340f5,
        (Api::IsUnitOnTheLeft)0x63e7f9,
        (Api::IsFlipped)0x63af35,
        (Api::GetBigFace)0x63b3c8,
        (Api::GetUnitRect)0x630b54,
        (Api::GetUnitRect)0x633c79,
        (Api::GetBoolById)0x63fad0,
        (Api::GetBoolById)0x63faf0,
        (Api::SetUnitId)0x650809,
        (Api::GetSelectedUnitId)0x64e04c,
    },
    // Russobit
    Api{
        (Api::MarkAttackTargets)0x6340f5,
        (Api::IsUnitOnTheLeft)0x63e7f9,
        (Api::IsFlipped)0x63af35,
        (Api::GetBigFace)0x63b3c8,
        (Api::GetUnitRect)0x630b54,
        (Api::GetUnitRect)0x633c79,
        (Api::GetBoolById)0x63fad0,
        (Api::GetBoolById)0x63faf0,
        (Api::SetUnitId)0x650809,
        (Api::GetSelectedUnitId)0x64e04c,
    },
    // Gog
    Api{
        (Api::MarkAttackTargets)0x632b35,
        (Api::IsUnitOnTheLeft)0x63d239,
        (Api::IsFlipped)0x639975,
        (Api::GetBigFace)0x639e08,
        (Api::GetUnitRect)0x62f594,
        (Api::GetUnitRect)0x6326b9,
        (Api::GetBoolById)0x63e4c0,
        (Api::GetBoolById)0x63e4e0,
        (Api::SetUnitId)0x64f149,
        (Api::GetSelectedUnitId)0x64c98c,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::BattleViewerInterfApi
