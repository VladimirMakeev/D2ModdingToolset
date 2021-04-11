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

#include "battlemsgdata.h"
#include "version.h"
#include <array>

namespace game::BattleMsgDataApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::GetUnitStatus)0x623b8c,
        (Api::SetUnitStatus)0x623c11,
        (Api::GetUnitIntValue)0x62332c,
        (Api::GetUnitIntValue)0x623391,
        (Api::SetUnitIntValue)0x6232d7,
        (Api::SetUnitInt16Value)0x62423b,
        (Api::CheckUnitDeath)0x65cf69,
        (Api::SetInt8Value)0x6247d9,
        (Api::AddSummonedUnit)0x6235c0,
        (Api::SetSummonOwner)0x6249ec,
        (Api::IsUnitTransformed)0x662e69,
        (Api::RemoveTransformStatuses)0x661f7b,
        (Api::IsUnitAttacker)0x623b58,
        (Api::GetUnitIntValue)0x6233f6,
        (Api::SetUnitIntValue)0x6233c3,
        (Api::SetUnitBoolValue)0x62371c,
        (Api::UnitHasModifier)0x65e9c8,
        (Api::GetUnitInfoById)0x622ef5,
        (Api::CanPerformAttackOnUnitWithStatusCheck)0x64734d,
        (Api::IsUnitAttackSourceWardRemoved)0x623d76,
        (Api::RemoveUnitAttackSourceWard)0x6240e9,
        (Api::IsUnitAttackClassWardRemoved)0x623dc7,
        (Api::RemoveUnitAttackClassWard)0x624130,
        (Api::UnitCanBeHealed)0x65e938,
        (Api::UnitCanBeCured)0x65d890,
        (Api::UnitCanBeRevived)0x65d927,
        (Api::SetUnknown9Bit1AndClearBoostLowerDamage)0x627ac9,
        (Api::BeforeAttack)0x627bff,
        (Api::ResetModifiedUnitsInfo)0x62326e,
        (Api::ResetUnitModifierInfo)0x6231d4,
    },
    // Russobit
    Api{
        (Api::GetUnitStatus)0x623b8c,
        (Api::SetUnitStatus)0x623c11,
        (Api::GetUnitIntValue)0x62332c,
        (Api::GetUnitIntValue)0x623391,
        (Api::SetUnitIntValue)0x6232d7,
        (Api::SetUnitInt16Value)0x62423b,
        (Api::CheckUnitDeath)0x65cf69,
        (Api::SetInt8Value)0x6247d9,
        (Api::AddSummonedUnit)0x6235c0,
        (Api::SetSummonOwner)0x6249ec,
        (Api::IsUnitTransformed)0x662e69,
        (Api::RemoveTransformStatuses)0x661f7b,
        (Api::IsUnitAttacker)0x623b58,
        (Api::GetUnitIntValue)0x6233f6,
        (Api::SetUnitIntValue)0x6233c3,
        (Api::SetUnitBoolValue)0x62371c,
        (Api::UnitHasModifier)0x65e9c8,
        (Api::GetUnitInfoById)0x622ef5,
        (Api::CanPerformAttackOnUnitWithStatusCheck)0x64734d,
        (Api::IsUnitAttackSourceWardRemoved)0x623d76,
        (Api::RemoveUnitAttackSourceWard)0x6240e9,
        (Api::IsUnitAttackClassWardRemoved)0x623dc7,
        (Api::RemoveUnitAttackClassWard)0x624130,
        (Api::UnitCanBeHealed)0x65e938,
        (Api::UnitCanBeCured)0x65d890,
        (Api::UnitCanBeRevived)0x65d927,
        (Api::SetUnknown9Bit1AndClearBoostLowerDamage)0x627ac9,
        (Api::BeforeAttack)0x627bff,
        (Api::ResetModifiedUnitsInfo)0x62326e,
        (Api::ResetUnitModifierInfo)0x6231d4,
    },
    // Gog
    Api{
        (Api::GetUnitStatus)0x62271c,
        (Api::SetUnitStatus)0x6227a1,
        (Api::GetUnitIntValue)0x621ebc,
        (Api::GetUnitIntValue)0x621f21,
        (Api::SetUnitIntValue)0x621e67,
        (Api::SetUnitInt16Value)0x622dcb,
        (Api::CheckUnitDeath)0x65b9e9,
        (Api::SetInt8Value)0x623369,
        (Api::AddSummonedUnit)0x622150,
        (Api::SetSummonOwner)0x62357c,
        (Api::IsUnitTransformed)0x6618e9,
        (Api::RemoveTransformStatuses)0x6609fb,
        (Api::IsUnitAttacker)0x6226e8,
        (Api::GetUnitIntValue)0x621f86,
        (Api::SetUnitIntValue)0x621f53,
        (Api::SetUnitBoolValue)0x6222ac,
        (Api::UnitHasModifier)0x65d448,
        (Api::GetUnitInfoById)0x621a85,
        (Api::CanPerformAttackOnUnitWithStatusCheck)0x645b7d,
        (Api::IsUnitAttackSourceWardRemoved)0x622906,
        (Api::RemoveUnitAttackSourceWard)0x622c79,
        (Api::IsUnitAttackClassWardRemoved)0x622957,
        (Api::RemoveUnitAttackClassWard)0x622cc0,
        (Api::UnitCanBeHealed)0x65d3b8,
        (Api::UnitCanBeCured)0x65c310,
        (Api::UnitCanBeRevived)0x65c3a7,
        (Api::SetUnknown9Bit1AndClearBoostLowerDamage)0x626609,
        (Api::BeforeAttack)0x62673f,
        (Api::ResetModifiedUnitsInfo)0x621dfe,
        (Api::ResetUnitModifierInfo)0x621d64,
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::BattleMsgDataApi
