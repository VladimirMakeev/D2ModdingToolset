/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "modifgroup.h"
#include "version.h"
#include <array>

namespace game::LModifGroupApi {

// clang-format off
static std::array<Categories, 4> cats = {{
    // Akella
    Categories{
        (LModifGroup*)0x83a388,
        (LModifGroup*)0x83a398,
        (LModifGroup*)0x83a3a8,
    },
    // Russobit
    Categories{
        (LModifGroup*)0x83a388,
        (LModifGroup*)0x83a398,
        (LModifGroup*)0x83a3a8,
    },
    // Gog
    Categories{
        (LModifGroup*)0x838338,
        (LModifGroup*)0x838348,
        (LModifGroup*)0x838358,
    },
    // Scenario Editor
    Categories{
        (LModifGroup*)0x665e70,
        (LModifGroup*)0x665e80,
        (LModifGroup*)0x665e90,
    },
}};

static std::array<void*, 4> vftables = {{
    // Akella
    (void*)0x6ea68c,
    // Russobit
    (void*)0x6ea68c,
    // Gog
    (void*)0x6e862c,
    // Scenario Editor
    (void*)0x5defa4,
}};
// clang-format on

Categories& categories()
{
    return cats[static_cast<int>(hooks::gameVersion())];
}

void* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::LModifGroupApi

namespace game::LModifGroupTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x59786a,
        (Api::Init)0x5979ba,
        (Api::ReadCategory)0x597a32,
        (Api::InitDone)0x597975,
        (Api::FindCategoryById)0x58af3b,
    },
    // Russobit
    Api{
        (Api::Constructor)0x59786a,
        (Api::Init)0x5979ba,
        (Api::ReadCategory)0x597a32,
        (Api::InitDone)0x597975,
        (Api::FindCategoryById)0x58af3b,
    },
    // Gog
    Api{
        (Api::Constructor)0x59698f,
        (Api::Init)0x596adf,
        (Api::ReadCategory)0x596b57,
        (Api::InitDone)0x596a9a,
        (Api::FindCategoryById)0x58a0a7,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x541029,
        (Api::Init)0x541179,
        (Api::ReadCategory)0x5411f1,
        (Api::InitDone)0x541134,
        (Api::FindCategoryById)0x535ccb,
    },
}};

static std::array<void*, 4> vftables = {{
    // Akella
    (void*)0x6eba5c,
    // Russobit
    (void*)0x6eba5c,
    // Gog
    (void*)0x6e99fc,
    // Scenario Editor
    (void*)0x5dfe8c,
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

void* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::LModifGroupTableApi
