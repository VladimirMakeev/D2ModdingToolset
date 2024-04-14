/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#include "testcondition.h"
#include "version.h"
#include <array>

namespace game::ITestConditionApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Create)0x4422b7,
        (ITestConditionVftable::Test)0x444d8f, // frequency
        (ITestConditionVftable::Test)0x444e22, // location
        (ITestConditionVftable::Test)0x44521f, // enter city
        (ITestConditionVftable::Test)0x4453d1, // own city
        (ITestConditionVftable::Test)0x4454cd, // kill stack
        (ITestConditionVftable::Test)0x44564a, // own item 
        (ITestConditionVftable::Test)0x445755, // leader own item
        (ITestConditionVftable::Test)0x4458fa, // diplomacy
        (ITestConditionVftable::Test)0x445a77, // alliance
        (ITestConditionVftable::Test)0x445b0a, // loot ruin
        (ITestConditionVftable::Test)0x445bbf, // transform land
        (ITestConditionVftable::Test)0x445d74, // visit site
        (ITestConditionVftable::Test)0x445044, // leader to zone
        (ITestConditionVftable::Test)0x4452d5, // leader to city
        (ITestConditionVftable::Test)0x445e9b, // item to location
        (ITestConditionVftable::Test)0x44609a, // stack exists
        (ITestConditionVftable::Test)0x446239, // var in range
    },
    // Russobit
    Api{
        (Api::Create)0x4422b7,
        (ITestConditionVftable::Test)0x444d8f, // frequency
        (ITestConditionVftable::Test)0x444e22, // location
        (ITestConditionVftable::Test)0x44521f, // enter city
        (ITestConditionVftable::Test)0x4453d1, // own city
        (ITestConditionVftable::Test)0x4454cd, // kill stack
        (ITestConditionVftable::Test)0x44564a, // own item 
        (ITestConditionVftable::Test)0x445755, // leader own item
        (ITestConditionVftable::Test)0x4458fa, // diplomacy
        (ITestConditionVftable::Test)0x445a77, // alliance
        (ITestConditionVftable::Test)0x445b0a, // loot ruin
        (ITestConditionVftable::Test)0x445bbf, // transform land
        (ITestConditionVftable::Test)0x445d74, // visit site
        (ITestConditionVftable::Test)0x445044, // leader to zone
        (ITestConditionVftable::Test)0x4452d5, // leader to city
        (ITestConditionVftable::Test)0x445e9b, // item to location
        (ITestConditionVftable::Test)0x44609a, // stack exists
        (ITestConditionVftable::Test)0x446239, // var in range
    },
    // Gog
    Api{
        (Api::Create)0x441f1c,
        (ITestConditionVftable::Test)0x444993, // frequency
        (ITestConditionVftable::Test)0x6fd6b8, // location
        (ITestConditionVftable::Test)0x444e23, // enter city
        (ITestConditionVftable::Test)0x444fd5, // own city
        (ITestConditionVftable::Test)0x4450d1, // kill stack
        (ITestConditionVftable::Test)0x44524e, // own item 
        (ITestConditionVftable::Test)0x445359, // leader own item
        (ITestConditionVftable::Test)0x4454fe, // diplomacy
        (ITestConditionVftable::Test)0x44567b, // alliance
        (ITestConditionVftable::Test)0x44570e, // loot ruin
        (ITestConditionVftable::Test)0x4457c3, // transform land
        (ITestConditionVftable::Test)0x445978, // visit site
        (ITestConditionVftable::Test)0x444c48, // leader to zone
        (ITestConditionVftable::Test)0x444ed9, // leader to city
        (ITestConditionVftable::Test)0x445a9f, // item to location
        (ITestConditionVftable::Test)0x445c9e, // stack exists
        (ITestConditionVftable::Test)0x445e3d, // var in range
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::ITestConditionApi
