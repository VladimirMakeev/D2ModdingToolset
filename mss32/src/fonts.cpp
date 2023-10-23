/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#include "fonts.h"
#include "version.h"
#include <array>

namespace game {

namespace FontListPtrPairApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::LoadFromFile)0x5cb3ad,
        (Api::IsLoaded)0x5298ce,
        (Api::DefaultCtor)0x5296db,
        (Api::Destructor)0x529802,
        (Api::CopyConstructor)0x529799,
    },
    // Russobit
    Api{
        (Api::LoadFromFile)0x5cb3ad,
        (Api::IsLoaded)0x5298ce,
        (Api::DefaultCtor)0x5296db,
        (Api::Destructor)0x529802,
        (Api::CopyConstructor)0x529799,
    },
    // Gog
    Api{
        (Api::LoadFromFile)0x5ca2c9,
        (Api::IsLoaded)0x528dd9,
        (Api::DefaultCtor)0x528be6,
        (Api::Destructor)0x528d0d,
        (Api::CopyConstructor)0x528ca4,
    },
    // Scenario Editor
    Api{
        (Api::LoadFromFile)0x59a822,
        (Api::IsLoaded)0x4c1165,
        (Api::DefaultCtor)0x4c0f72,
        (Api::Destructor)0x4c1099,
        (Api::CopyConstructor)0x4c1030,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace FontListPtrPairApi

namespace FontCacheApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::LoadFontFiles)0x5cb123,
        (Api::DataConstructor)0x5cb44c,
        (Api::DataDestructor)0x5cb4d7,
        (Api::SmartPtrSetData)0x4ffda8,
        (FontCachePtr*)0x83a980,
    },
    // Russobit
    Api{
        (Api::LoadFontFiles)0x5cb123,
        (Api::DataConstructor)0x5cb44c,
        (Api::DataDestructor)0x5cb4d7,
        (Api::SmartPtrSetData)0x4ffda8,
        (FontCachePtr*)0x83a980,
    },
    // Gog
    Api{
        (Api::LoadFontFiles)0x5ca03f,
        (Api::DataConstructor)0x5ca368,
        (Api::DataDestructor)0x5ca3f3,
        (Api::SmartPtrSetData)0x4ff0aa,
        (FontCachePtr*)0x838928,
    },
    // Scenario Editor
    Api{
        (Api::LoadFontFiles)0x59a598,
        (Api::DataConstructor)0x59a8c1,
        (Api::DataDestructor)0x59a94c,
        (Api::SmartPtrSetData)0x51ea55,
        (FontCachePtr*)0x666b90,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace FontCacheApi

} // namespace game
