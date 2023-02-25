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
static std::array<Api, 4> functions = {{
    // Akella
    Api {
        (Api::DynamicCast)0x66d466,
        (Api::TypeInfoRawName*)0x6ce2cc,
        (Api::TypeInfoInequalityOperator*)0x6ce1a0,
        (Api::TypeIdOperator*)0x6ce2b4,
    },
    // Russobit
    Api {
        (Api::DynamicCast)0x66d466,
        (Api::TypeInfoRawName*)0x6ce2cc,
        (Api::TypeInfoInequalityOperator*)0x6ce1a0,
        (Api::TypeIdOperator*)0x6ce2b4,
    },
    // Gog
    Api {
        (Api::DynamicCast)0x66bda6,
        (Api::TypeInfoRawName*)0x6cc25c,
        (Api::TypeInfoInequalityOperator*)0x6cc1fc,
        (Api::TypeIdOperator*)0x6cc2c8,
    },
    // Scenario Editor
    Api {
        (Api::DynamicCast)0x57109c,
        (Api::TypeInfoRawName*)0x5ca258,
        (Api::TypeInfoInequalityOperator*)0x5ca15c,
        (Api::TypeIdOperator*)0x5ca1f0,
    },
}};

static const std::array<Rtti, 4> types = {{
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
        (TypeDescriptor*)0x7b28e8,
        (TypeDescriptor*)0x7aeea0,
        (TypeDescriptor*)0x7b0150,
        (TypeDescriptor*)0x7b01a0,
        (TypeDescriptor*)0x799da0,
        (TypeDescriptor*)0x799db8,
        (TypeDescriptor*)0x78f0f8,
        (TypeDescriptor*)0x7b0008,
        (TypeDescriptor*)0x7b4620,
        (TypeDescriptor*)0x7b51e8,
        (TypeDescriptor*)0x793c38,
        (TypeDescriptor*)0x7af710,
        (TypeDescriptor*)0x78f118,
        (TypeDescriptor*)0x7acdc8,
        (TypeDescriptor*)0x7a1178,
        (TypeDescriptor*)0x7ad348,
        (TypeDescriptor*)0x7b0138,
        (TypeDescriptor*)0x7ad2c8,
        (TypeDescriptor*)0x7a10f8,
        (TypeDescriptor*)0x7b52d0,
        (TypeDescriptor*)0x7b52f0,
        (TypeDescriptor*)0x7afb80, // CItemEquipmentType
        (TypeDescriptor*)0x7b0108, // CUmStackType
        (TypeDescriptor*)0x78ee68, // CFortificationType
        (TypeDescriptor*)0x799ec0, // IMidObjectType
        (TypeDescriptor*)0x793d90, // IUsNobleType
        (TypeDescriptor*)0x7974b8, // IUsSummonType
        (TypeDescriptor*)0x7afcf8, // IItemExPotionBoostType
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
        (TypeDescriptor*)0x7b28e8,
        (TypeDescriptor*)0x7aeea0,
        (TypeDescriptor*)0x7b0150,
        (TypeDescriptor*)0x7b01a0,
        (TypeDescriptor*)0x799da0,
        (TypeDescriptor*)0x799db8,
        (TypeDescriptor*)0x78f0f8,
        (TypeDescriptor*)0x7b0008,
        (TypeDescriptor*)0x7b4620,
        (TypeDescriptor*)0x7b51e8,
        (TypeDescriptor*)0x793c38,
        (TypeDescriptor*)0x7af710,
        (TypeDescriptor*)0x78f118,
        (TypeDescriptor*)0x7acdc8,
        (TypeDescriptor*)0x7a1178,
        (TypeDescriptor*)0x7ad348,
        (TypeDescriptor*)0x7b0138,
        (TypeDescriptor*)0x7ad2c8,
        (TypeDescriptor*)0x7a10f8,
        (TypeDescriptor*)0x7b52d0,
        (TypeDescriptor*)0x7b52f0,
        (TypeDescriptor*)0x7afb80, // CItemEquipmentType
        (TypeDescriptor*)0x7b0108, // CUmStackType
        (TypeDescriptor*)0x78ee68, // CFortificationType
        (TypeDescriptor*)0x799ec0, // IMidObjectType
        (TypeDescriptor*)0x793d90, // IUsNobleType
        (TypeDescriptor*)0x7974b8, // IUsSummonType
        (TypeDescriptor*)0x7afcf8, // IItemExPotionBoostType
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
        (TypeDescriptor*)0x7b0898,
        (TypeDescriptor*)0x7ace58,
        (TypeDescriptor*)0x7ae108,
        (TypeDescriptor*)0x7ae158,
        (TypeDescriptor*)0x797d48,
        (TypeDescriptor*)0x797d60,
        (TypeDescriptor*)0x78d0a0,
        (TypeDescriptor*)0x7adfc0,
        (TypeDescriptor*)0x7b25d0,
        (TypeDescriptor*)0x7b3198,
        (TypeDescriptor*)0x791be0,
        (TypeDescriptor*)0x7ad6c8,
        (TypeDescriptor*)0x78d0c0,
        (TypeDescriptor*)0x7aad80,
        (TypeDescriptor*)0x79f118,
        (TypeDescriptor*)0x7ab300,
        (TypeDescriptor*)0x7ae0f0,
        (TypeDescriptor*)0x7ab280,
        (TypeDescriptor*)0x79f0a0,
        (TypeDescriptor*)0x7b3280,
        (TypeDescriptor*)0x7b32a0,
        (TypeDescriptor*)0x7adb38, // CItemEquipmentType
        (TypeDescriptor*)0x7ae0c0, // CUmStackType
        (TypeDescriptor*)0x78ce10, // CFortificationType
        (TypeDescriptor*)0x797e68, // IMidObjectType
        (TypeDescriptor*)0x791d38, // IUsNobleType
        (TypeDescriptor*)0x795460, // IUsSummonType
        (TypeDescriptor*)0x7adcb0, // IItemExPotionBoostType
    },
    // Scenario Editor
    Rtti{
        (TypeDescriptor*)0x648b48,
        (TypeDescriptor*)0x649788,
        (TypeDescriptor*)0x6561a8, // CPlayerBuildingsType
        (TypeDescriptor*)0x655d40, // TBuildingTypeType
        (TypeDescriptor*)0x6561e8, // TBuildingUnitUpgTypeType
        (TypeDescriptor*)0x648b90,
        (TypeDescriptor*)0x656e08,
        (TypeDescriptor*)0x65bd20,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)0x656310,
        (TypeDescriptor*)0x65a5a8,
        (TypeDescriptor*)0x65bd50,
        (TypeDescriptor*)0x65bd70,
        (TypeDescriptor*)0x655a90,
        (TypeDescriptor*)0x65b0d0,
        (TypeDescriptor*)0x648e98,
        (TypeDescriptor*)0x65bc28,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)0x649768,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)0x649398,
        (TypeDescriptor*)0x655848,
        (TypeDescriptor*)0x655d80,
        (TypeDescriptor*)0x655e28,
        (TypeDescriptor*)0x65bd38,
        (TypeDescriptor*)0x655da8,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)nullptr,
        (TypeDescriptor*)0x65b820, // CItemEquipmentType
        (TypeDescriptor*)0x65bd08, // CUmStackType
        (TypeDescriptor*)0x648e20, // CFortificationType
        (TypeDescriptor*)0x649438, // IMidObjectType
        (TypeDescriptor*)0x64c3d0, // IUsNobleType
        (TypeDescriptor*)0x64f068, // IUsSummonType
        (TypeDescriptor*)0x656e28, // IItemExPotionBoostType
    },
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
