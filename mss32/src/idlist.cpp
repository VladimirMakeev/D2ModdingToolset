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

#include "idlist.h"
#include "version.h"
#include <array>

namespace game::IdListApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::Clear)0x56ab57,
        (Api::PushBack)0x5c9ebd,
        (Api::Erase)0x408017,
        (Api::GetIterator)0x4dee1a,
        (Api::GetIterator)0x470bec,
        (Api::Find)0x40b8e2,
        (Api::IteratorEquals)0x474a74,
        (Api::Shuffle)0x46b079,
    },
    // Russobit
    Api{
        (Api::Clear)0x56ab57,
        (Api::PushBack)0x5c9ebd,
        (Api::Erase)0x408017,
        (Api::GetIterator)0x4dee1a,
        (Api::GetIterator)0x470bec,
        (Api::Find)0x40b8e2,
        (Api::IteratorEquals)0x474a74,
        (Api::Shuffle)0x46b079,
    },
    // Gog
    Api{
        (Api::Clear)0x42c2c1,
        (Api::PushBack)0x56aff7,
        (Api::Erase)0x407ca2,
        (Api::GetIterator)0x4c7e28,
        (Api::GetIterator)0x40b43f,
        (Api::Find)0x40b5c0,
        (Api::IteratorEquals)0x5d8fbb,
        (Api::Shuffle)0x46a9f5,
    },
    // Scenario Editor
    Api{
        (Api::Clear)0x470799,
        (Api::PushBack)0x49bb17,
        (Api::Erase)0x408987,
        (Api::GetIterator)0x4bf530,
        (Api::GetIterator)0x486901,
        (Api::Find)0x440cd1,
        (Api::IteratorEquals)0x41ec65,
        (Api::Shuffle)0x471a79,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::IdListApi
