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

#ifndef D2UNORDEREDMAP_H
#define D2UNORDEREDMAP_H

#include "d2assert.h"
#include <cstdint>

namespace game {

template <typename Key, typename T>
struct UnorderedMapBucket
{
    Key key;
    T value;
    std::uint32_t partialHash;
    UnorderedMapBucket<Key, T>* next;
};

using AssertBucketSize = UnorderedMapBucket<int, int>;
assert_size(AssertBucketSize, 16);

/** Implementation of std::unordered_map<Key, T> used in game. */
template <typename Key, typename T, typename Unknown = int>
struct UnorderedMap
{
    char unknown;
    char padding[3];
    std::uint32_t elementsCount;
    UnorderedMapBucket<Key, T>** buckets;
    std::uint32_t bucketsCount;
    std::uint32_t reallocationThreshold;
    int maxLoadFactor; /**< Bit representation of a float value */
    char unknown4;
    char unknown5;
    char unknown6;
    char unknown7;
    Unknown unknown8;
    int unknown9;
    void* allocator;
};

using AssertUnorderedMapSize = UnorderedMap<int, int>;
assert_size(AssertUnorderedMapSize, 40);

} // namespace game

#endif // D2UNORDEREDMAP_H
