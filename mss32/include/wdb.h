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

#ifndef WDB_H
#define WDB_H

#include "d2assert.h"
#include "d2pair.h"
#include "mq_c_s.h"
#include "quickstring.h"
#include <cstdint>

namespace game {

struct MQDBFile;

using WdbNameIdPair = Pair<quick_string<16u>, std::uint32_t>;
using WdbNamesArray = mq_c_s<WdbNameIdPair>;

struct WdbData
{
    MQDBFile** mqdb;
    WdbNamesArray namesArray;
};

assert_size(WdbData, 24);

/** Stores information of .wdb file. */
struct Wdb
{
    WdbData* data;
};

assert_size(Wdb, 4);

} // namespace game

#endif // WDB_H
