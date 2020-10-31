/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#include "currency.h"
#include "version.h"
#include <array>

namespace game::BankApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Add)0x58748f,
        (Api::Subtract)0x5873c5,
        (Api::Multiply)0x587559,
        (Api::Divide)0x58763d,
        (Api::Less)0x587833,
        (Api::Copy)0x5870dd,
        (Api::SetInvalid)0x5870ac,
        (Api::SetZero)0x5879a7,
        (Api::SetCurrency)0x5878bc,
        (Api::IsZero)0x58797e,
        (Api::IsValid)0x58790f,
        (Api::ToString)0x5872d9,
        (Api::FromString)0x5873ab
    },
    // Russobit
    Api{
        (Api::Add)0x58748f,
        (Api::Subtract)0x5873c5,
        (Api::Multiply)0x587559,
        (Api::Divide)0x58763d,
        (Api::Less)0x587833,
        (Api::Copy)0x5870dd,
        (Api::SetInvalid)0x5870ac,
        (Api::SetZero)0x5879a7,
        (Api::SetCurrency)0x5878bc,
        (Api::IsZero)0x58797e,
        (Api::IsValid)0x58790f,
        (Api::ToString)0x5872d9,
        (Api::FromString)0x5873ab
    },
    // Gog
    Api{
        (Api::Add)0x586642,
        (Api::Subtract)0x586578,
        (Api::Multiply)0x58670c,
        (Api::Divide)0x5867f0,
        (Api::Less)0x5869e6,
        (Api::Copy)0x586290,
        (Api::SetInvalid)0x58625f,
        (Api::SetZero)0x586b5a,
        (Api::SetCurrency)0x586a6f,
        (Api::IsZero)0x586b31,
        (Api::IsValid)0x586ac2,
        (Api::ToString)0x58648c,
        (Api::FromString)0x58655e
    }
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::BankApi
