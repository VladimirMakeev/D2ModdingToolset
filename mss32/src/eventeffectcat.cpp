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

#include "eventeffectcat.h"
#include "version.h"
#include <array>

namespace game {

namespace EventEffectCategories {

// clang-format off
static std::array<Categories, 4> categories = {{
    // Akella
    Categories{
        (LEventEffectCategory*)0x839c58,
        (LEventEffectCategory*)0x839cf8,
        (LEventEffectCategory*)0x839c98,
        (LEventEffectCategory*)0x839ce8,
        (LEventEffectCategory*)0x839d48,
        (LEventEffectCategory*)0x839cb8,
        (LEventEffectCategory*)0x839c18,
        (LEventEffectCategory*)0x839ca8,
        (LEventEffectCategory*)0x839c68,
        (LEventEffectCategory*)0x839bf8,
        (LEventEffectCategory*)0x839c88,
        (LEventEffectCategory*)0x839c78,
        (LEventEffectCategory*)0x839cc8,
        (LEventEffectCategory*)0x839d28,
        (LEventEffectCategory*)0x839d08,
        (LEventEffectCategory*)0x839c28,
        (LEventEffectCategory*)0x839c48,
        (LEventEffectCategory*)0x839c08,
        (LEventEffectCategory*)0x839be8,
        (LEventEffectCategory*)0x839c38,
        (LEventEffectCategory*)0x839d38,
        (LEventEffectCategory*)0x839d18,
        (LEventEffectCategory*)0x839cd8,
        (LEventEffectCategory*)0x839d58,
    },
    // Russobit
    Categories{
        (LEventEffectCategory*)0x839c58,
        (LEventEffectCategory*)0x839cf8,
        (LEventEffectCategory*)0x839c98,
        (LEventEffectCategory*)0x839ce8,
        (LEventEffectCategory*)0x839d48,
        (LEventEffectCategory*)0x839cb8,
        (LEventEffectCategory*)0x839c18,
        (LEventEffectCategory*)0x839ca8,
        (LEventEffectCategory*)0x839c68,
        (LEventEffectCategory*)0x839bf8,
        (LEventEffectCategory*)0x839c88,
        (LEventEffectCategory*)0x839c78,
        (LEventEffectCategory*)0x839cc8,
        (LEventEffectCategory*)0x839d28,
        (LEventEffectCategory*)0x839d08,
        (LEventEffectCategory*)0x839c28,
        (LEventEffectCategory*)0x839c48,
        (LEventEffectCategory*)0x839c08,
        (LEventEffectCategory*)0x839be8,
        (LEventEffectCategory*)0x839c38,
        (LEventEffectCategory*)0x839d38,
        (LEventEffectCategory*)0x839d18,
        (LEventEffectCategory*)0x839cd8,
        (LEventEffectCategory*)0x839d58,
    },
    // Gog
    Categories{
        (LEventEffectCategory*)0x837c08,
        (LEventEffectCategory*)0x837ca8,
        (LEventEffectCategory*)0x837c48,
        (LEventEffectCategory*)0x837c98,
        (LEventEffectCategory*)0x837cf8,
        (LEventEffectCategory*)0x837c68,
        (LEventEffectCategory*)0x837bc8,
        (LEventEffectCategory*)0x837c58,
        (LEventEffectCategory*)0x837c18,
        (LEventEffectCategory*)0x837ba8,
        (LEventEffectCategory*)0x837c38,
        (LEventEffectCategory*)0x837c28,
        (LEventEffectCategory*)0x837c78,
        (LEventEffectCategory*)0x837cd8,
        (LEventEffectCategory*)0x837cb8,
        (LEventEffectCategory*)0x837bd8,
        (LEventEffectCategory*)0x837bf8,
        (LEventEffectCategory*)0x837bb8,
        (LEventEffectCategory*)0x837b98,
        (LEventEffectCategory*)0x837be8,
        (LEventEffectCategory*)0x837ce8,
        (LEventEffectCategory*)0x837cc8,
        (LEventEffectCategory*)0x837c88,
        (LEventEffectCategory*)0x837d08,
    },
    // Scenario Editor
    Categories{
        (LEventEffectCategory*)0x6654a8,
        (LEventEffectCategory*)0x665548,
        (LEventEffectCategory*)0x6654e8,
        (LEventEffectCategory*)0x665538,
        (LEventEffectCategory*)0x665598,
        (LEventEffectCategory*)0x665508,
        (LEventEffectCategory*)0x665468,
        (LEventEffectCategory*)0x6654f8,
        (LEventEffectCategory*)0x6654b8,
        (LEventEffectCategory*)0x665448,
        (LEventEffectCategory*)0x6654d8,
        (LEventEffectCategory*)0x6654c8,
        (LEventEffectCategory*)0x665518,
        (LEventEffectCategory*)0x665578,
        (LEventEffectCategory*)0x665558,
        (LEventEffectCategory*)0x665478,
        (LEventEffectCategory*)0x665498,
        (LEventEffectCategory*)0x665458,
        (LEventEffectCategory*)0x665438,
        (LEventEffectCategory*)0x665488,
        (LEventEffectCategory*)0x665588,
        (LEventEffectCategory*)0x665568,
        (LEventEffectCategory*)0x665528,
        (LEventEffectCategory*)0x6655a8,
    },
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6eac24,
    // Russobit
    (const void*)0x6eac24,
    // Gog
    (const void*)0x6e8bc4,
    // Scenario Editor
    (const void*)0x5cf014,
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

} // namespace EventEffectCategories

namespace LEventEffectCategoryTableApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x58e57a,
        (Api::Init)0x58e8ad,
        (Api::ReadCategory)0x58e925,
        (Api::InitDone)0x58e868,
        (Api::FindCategoryById)nullptr,
    },
    // Russobit
    Api{
        (Api::Constructor)0x58e57a,
        (Api::Init)0x58e8ad,
        (Api::ReadCategory)0x58e925,
        (Api::InitDone)0x58e868,
        (Api::FindCategoryById)nullptr,
    },
    // Gog
    Api{
        (Api::Constructor)0x58d68f,
        (Api::Init)0x58d9c2,
        (Api::ReadCategory)0x58da3a,
        (Api::InitDone)0x58d97d,
        (Api::FindCategoryById)nullptr,
    },
    // Scenario Editor
    Api{
        (Api::Constructor)0x532bd7,
        (Api::Init)0x532f0a,
        (Api::ReadCategory)0x532f82,
        (Api::InitDone)0x532ec5,
        (Api::FindCategoryById)nullptr,
    },
}};

static std::array<const void*, 4> vftables = {{
    // Akella
    (const void*)0x6eac2c,
    // Russobit
    (const void*)0x6eac2c,
    // Gog
    (const void*)0x6e8bcc,
    // Scenario Editor
    (const void*)0x5de9cc,
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

} // namespace LEventEffectCategoryTableApi

} // namespace game
