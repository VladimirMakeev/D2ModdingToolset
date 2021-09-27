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
        (Api::GetBigFace)0x63b349,
        (Api::GetUnitRect)0x630b54,
        (Api::GetUnitRect)0x633c79,
        (Api::GetBoolById)0x63fad0,
        (Api::GetBoolById)0x63faf0,
        (Api::GetBoolById)0x63f980,
        (Api::SetUnitId)0x650809,
        (Api::SetUnknown)0x651124,
        (Api::GetSelectedUnitId)0x64e04c,
        (Api::HighlightGroupFrame)0x64e289,
        (Api::UnknownMethod)0x64e17e,
        (Api::UnknownMethod2)0x64e204,
        (Api::UnknownMethod3)0x64e4dc,
        (Api::GetUnitAnimation)0x63b447,
        (Api::UpdateUnknown)0x639743,
        (Api::UpdateUnknown)0x639990,
        (Api::CBattleViewerTargetDataSetConstructor)0x63f800,
        (Api::CBattleViewerTargetDataSetSetAttacker)0x63f890,
        (Api::Callback)0x6355ef,
        (Api::Callback)0x635664,
        (Api::CreateButtonFunctor)0x643370,
        (Api::UnknownMethod4)0x643040,
        (Api::FillTargetPositions)0x631f08,
        (Api::AddTargetUnitData)0x6388a7,
        (Api::SetCheckedForRightUnitsToggleButton)0x631ee1,
        (Api::UnknownMethod6)0x639858,
        (Api::UnknownMethod7)0x638f65,
        (Api::UnknownMethod8)0x634fdf,
    },
    // Russobit
    Api{
        (Api::MarkAttackTargets)0x6340f5,
        (Api::IsUnitOnTheLeft)0x63e7f9,
        (Api::IsFlipped)0x63af35,
        (Api::GetBigFace)0x63b3c8,
        (Api::GetBigFace)0x63b349,
        (Api::GetUnitRect)0x630b54,
        (Api::GetUnitRect)0x633c79,
        (Api::GetBoolById)0x63fad0,
        (Api::GetBoolById)0x63faf0,
        (Api::GetBoolById)0x63f980,
        (Api::SetUnitId)0x650809,
        (Api::SetUnknown)0x651124,
        (Api::GetSelectedUnitId)0x64e04c,
        (Api::HighlightGroupFrame)0x64e289,
        (Api::UnknownMethod)0x64e17e,
        (Api::UnknownMethod2)0x64e204,
        (Api::UnknownMethod3)0x64e4dc,
        (Api::GetUnitAnimation)0x63b447,
        (Api::UpdateUnknown)0x639743,
        (Api::UpdateUnknown)0x639990,
        (Api::CBattleViewerTargetDataSetConstructor)0x63f800,
        (Api::CBattleViewerTargetDataSetSetAttacker)0x63f890,
        (Api::Callback)0x6355ef,
        (Api::Callback)0x635664,
        (Api::CreateButtonFunctor)0x643370,
        (Api::UnknownMethod4)0x643040,
        (Api::FillTargetPositions)0x631f08,
        (Api::AddTargetUnitData)0x6388a7,
        (Api::SetCheckedForRightUnitsToggleButton)0x631ee1,
        (Api::UnknownMethod6)0x639858,
        (Api::UnknownMethod7)0x638f65,
        (Api::UnknownMethod8)0x634fdf,
    },
    // Gog
    Api{
        (Api::MarkAttackTargets)0x632b35,
        (Api::IsUnitOnTheLeft)0x63d239,
        (Api::IsFlipped)0x639975,
        (Api::GetBigFace)0x639e08,
        (Api::GetBigFace)0x639d89,
        (Api::GetUnitRect)0x62f594,
        (Api::GetUnitRect)0x6326b9,
        (Api::GetBoolById)0x63e4c0,
        (Api::GetBoolById)0x63e4e0,
        (Api::GetBoolById)0x63e370,
        (Api::SetUnitId)0x64f149,
        (Api::SetUnknown)0x64fa64,
        (Api::GetSelectedUnitId)0x64c98c,
        (Api::HighlightGroupFrame)0x64cbc9,
        (Api::UnknownMethod)0x64cabe,
        (Api::UnknownMethod2)0x64cb44,
        (Api::UnknownMethod3)0x64ce1c,
        (Api::GetUnitAnimation)0x639e87,
        (Api::UpdateUnknown)0x638183,
        (Api::UpdateUnknown)0x6383d0,
        (Api::CBattleViewerTargetDataSetConstructor)0x63e1f0,
        (Api::CBattleViewerTargetDataSetSetAttacker)0x63e280,
        (Api::Callback)0x63402f,
        (Api::Callback)0x6340a4,
        (Api::CreateButtonFunctor)0x641bd0,
        (Api::UnknownMethod4)0x641890,
        (Api::FillTargetPositions)0x630948,
        (Api::AddTargetUnitData)0x6372e7,
        (Api::SetCheckedForRightUnitsToggleButton)0x630921,
        (Api::UnknownMethod6)0x638298,
        (Api::UnknownMethod7)0x6379a5,
        (Api::UnknownMethod8)0x633a1f,
    },
}};

static std::array<IBatViewerVftable*, 4> vftables = {{
    // Akella
    (IBatViewerVftable*)0x6f4294,
    // Russobit
    (IBatViewerVftable*)0x6f4294,
    // Gog
    (IBatViewerVftable*)0x6f2244,
    // Scenario Editor
    (IBatViewerVftable*)nullptr,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

const IBatViewerVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::BattleViewerInterfApi
