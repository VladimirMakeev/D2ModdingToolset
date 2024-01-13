/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#include "sitecategories.h"
#include "version.h"
#include <array>

namespace game {

namespace SiteCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LSiteCategory*)0x83a290,
        (LSiteCategory*)0x83a2a8,
        (LSiteCategory*)0x83a270,
        (LSiteCategory*)0x83a280,
    },
    // Russobit
    Categories{
        (LSiteCategory*)0x83a290,
        (LSiteCategory*)0x83a2a8,
        (LSiteCategory*)0x83a270,
        (LSiteCategory*)0x83a280,
    },
    // Gog
    Categories{
        (LSiteCategory*)0x838240,
        (LSiteCategory*)0x838258,
        (LSiteCategory*)0x838220,
        (LSiteCategory*)0x838230,
    },
    // Scenario Editor
    Categories{
        (LSiteCategory*)0x6651a8,
        (LSiteCategory*)0x6651c0,
        (LSiteCategory*)0x665188,
        (LSiteCategory*)0x665198,
    }
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6e88d4,
    // Russobit
    (const void*)0x6e88d4,
    // Gog
    (const void*)0x6e6874,
    // Scenario Editor
    (const void*)0x5cb054,
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

} // namespace SiteCategories

namespace LSiteCategoryTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x594f45,
        (Api::Init)0x5950ac,
        (Api::ReadCategory)0x595124,
        (Api::InitDone)0x595067,
        (Api::FindCategoryById)nullptr,
    },
    // Russobit
    Api{
        (Api::Constructor)0x594f45,
        (Api::Init)0x5950ac,
        (Api::ReadCategory)0x595124,
        (Api::InitDone)0x595067,
        (Api::FindCategoryById)nullptr,
    },
    // Gog
    Api{
        (Api::Constructor)0x594035,
        (Api::Init)0x59419c,
        (Api::ReadCategory)0x594214,
        (Api::InitDone)0x594157,
        (Api::FindCategoryById)nullptr,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x52F2CA,
        (Api::Init)0x52F431,
        (Api::ReadCategory)0x52F4A9,
        (Api::InitDone)0x52F3EC,
        (Api::FindCategoryById)nullptr,
    }
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6eb6d4,
    // Russobit
    (const void*)0x6eb6d4,
    // Gog
    (const void*)0x6e9674,
    // Scenario Editor
    (const void*)0x5de02c,
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

} // namespace LSiteCategoryTableApi

} // namespace game
