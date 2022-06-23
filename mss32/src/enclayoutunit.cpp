/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#include "enclayoutunit.h"
#include "version.h"
#include <array>

namespace game::CEncLayoutUnitApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x574e9c,
        (Api::Constructor2)0x574fd5,
        (Api::DataConstructor)0x574f48,
        (Api::DataDestructor)0x577d72,
        (Api::Initialize)0x5750b1,
        (Api::Update)0x5757eb,
        (Api::CreateListBoxDisplayFunctor)0x577b6d,
    },
    // Russobit
    Api{
        (Api::Constructor)0x574e9c,
        (Api::Constructor2)0x574fd5,
        (Api::DataConstructor)0x574f48,
        (Api::DataDestructor)0x577d72,
        (Api::Initialize)0x5750b1,
        (Api::Update)0x5757eb,
        (Api::CreateListBoxDisplayFunctor)0x577b6d,
    },
    // Gog
    Api{
        (Api::Constructor)0x5744f1,
        (Api::Constructor2)0x57462a,
        (Api::DataConstructor)0x57459d,
        (Api::DataDestructor)0x57742d,
        (Api::Initialize)0x574706,
        (Api::Update)0x574e40,
        (Api::CreateListBoxDisplayFunctor)0x577228,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x4c5915,
        (Api::Constructor2)0x4c5a71,
        (Api::DataConstructor)0x4c59c1,
        (Api::DataDestructor)0x4c8d48,
        (Api::Initialize)0x4c5b4d,
        (Api::Update)0x4c6287,
        (Api::CreateListBoxDisplayFunctor)0x4c885f,
    },
}};

static std::array<IEncLayoutVftable*, 4> vftables = {{
    // Akella
    (IEncLayoutVftable*)0x6e7f7c,
    // Russobit
    (IEncLayoutVftable*)0x6e7f7c,
    // Gog
    (IEncLayoutVftable*)0x6e5f1c,
    // Scenario Editor
    (IEncLayoutVftable*)0x5d72bc,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

IEncLayoutVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CEncLayoutUnitApi
