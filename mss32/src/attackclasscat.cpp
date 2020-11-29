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

#include "attackclasscat.h"
#include "version.h"
#include <array>

namespace game {
namespace AttackClassCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LAttackClass*)0x8394d0,
        (LAttackClass*)0x839590,
        (LAttackClass*)0x839470,
        (LAttackClass*)0x8395c8,
        (LAttackClass*)0x8394e0,
        (LAttackClass*)0x839480,
        (LAttackClass*)0x839510,
        (LAttackClass*)0x8394f0,
        (LAttackClass*)0x839560,
        (LAttackClass*)0x839540,
        (LAttackClass*)0x8395a0,
        (LAttackClass*)0x839490,
        (LAttackClass*)0x839500,
        (LAttackClass*)0x839520,
        (LAttackClass*)0x839550,
        (LAttackClass*)0x8394a0,
        (LAttackClass*)0x839580,
        (LAttackClass*)0x839530,
        (LAttackClass*)0x8395b8,
        (LAttackClass*)0x8394b0,
        (LAttackClass*)0x839570,
        (LAttackClass*)0x8394c0,
        (LAttackClass*)0x8395d8
    },
    // Russobit
    Categories{
        (LAttackClass*)0x8394d0,
        (LAttackClass*)0x839590,
        (LAttackClass*)0x839470,
        (LAttackClass*)0x8395c8,
        (LAttackClass*)0x8394e0,
        (LAttackClass*)0x839480,
        (LAttackClass*)0x839510,
        (LAttackClass*)0x8394f0,
        (LAttackClass*)0x839560,
        (LAttackClass*)0x839540,
        (LAttackClass*)0x8395a0,
        (LAttackClass*)0x839490,
        (LAttackClass*)0x839500,
        (LAttackClass*)0x839520,
        (LAttackClass*)0x839550,
        (LAttackClass*)0x8394a0,
        (LAttackClass*)0x839580,
        (LAttackClass*)0x839530,
        (LAttackClass*)0x8395b8,
        (LAttackClass*)0x8394b0,
        (LAttackClass*)0x839570,
        (LAttackClass*)0x8394c0,
        (LAttackClass*)0x8395d8
    },
    // Gog
    Categories{
        (LAttackClass*)0x837480,
        (LAttackClass*)0x837540,
        (LAttackClass*)0x837420,
        (LAttackClass*)0x837578,
        (LAttackClass*)0x837490,
        (LAttackClass*)0x837430,
        (LAttackClass*)0x8374c0,
        (LAttackClass*)0x8374a0,
        (LAttackClass*)0x837510,
        (LAttackClass*)0x8374f0,
        (LAttackClass*)0x837550,
        (LAttackClass*)0x837440,
        (LAttackClass*)0x8374b0,
        (LAttackClass*)0x8374d0,
        (LAttackClass*)0x837500,
        (LAttackClass*)0x837450,
        (LAttackClass*)0x837530,
        (LAttackClass*)0x8374e0,
        (LAttackClass*)0x837568,
        (LAttackClass*)0x837460,
        (LAttackClass*)0x837520,
        (LAttackClass*)0x837470,
        (LAttackClass*)0x837588
    },
    // Scenario Editor
    Categories{
        (LAttackClass*)0x665728,
        (LAttackClass*)0x6657e8,
        (LAttackClass*)0x6656c8,
        (LAttackClass*)0x665820,
        (LAttackClass*)0x665738,
        (LAttackClass*)0x6656d8,
        (LAttackClass*)0x665768,
        (LAttackClass*)0x665748,
        (LAttackClass*)0x6657b8,
        (LAttackClass*)0x665798,
        (LAttackClass*)0x6657f8,
        (LAttackClass*)0x6656e8,
        (LAttackClass*)0x665758,
        (LAttackClass*)0x665778,
        (LAttackClass*)0x6657a8,
        (LAttackClass*)0x6656f8,
        (LAttackClass*)0x6657d8,
        (LAttackClass*)0x665788,
        (LAttackClass*)0x665810,
        (LAttackClass*)0x665708,
        (LAttackClass*)0x6657c8,
        (LAttackClass*)0x665718,
        (LAttackClass*)0x665830
    }
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6ceadc,
    // Russobit
    (const void*)0x6ceadc,
    // Gog
    (const void*)0x6cca7c,
    // Scenario Editor
    (const void*)0x5d295c
}};
// clang-format on

Categories& get()
{
    return categories[static_cast<int>(hooks::gameVersion())];
}

const void* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace AttackClassCategories

namespace LAttackClassTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x585a1a,
        (Api::Init)0x585d36,
        (Api::ReadCategory)0x585dae,
        (Api::InitDone)0x585cf1
    },
    // Russobit
    Api{
        (Api::Constructor)0x585a1a,
        (Api::Init)0x585d36,
        (Api::ReadCategory)0x585dae,
        (Api::InitDone)0x585cf1
    },
    // Gog
    Api{
        (Api::Constructor)0x584bcd,
        (Api::Init)0x584ee9,
        (Api::ReadCategory)0x584f61,
        (Api::InitDone)0x584ea4
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x536288,
        (Api::Init)0x5365a4,
        (Api::ReadCategory)0x53661c,
        (Api::InitDone)0x53655f
    }
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6e9eb4,
    // Russobit
    (const void*)0x6e9eb4,
    // Gog
    (const void*)0x6e7e54,
    // Scenario Editor
    (const void*)0x5df10c
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

const void* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace LAttackClassTableApi

} // namespace game
