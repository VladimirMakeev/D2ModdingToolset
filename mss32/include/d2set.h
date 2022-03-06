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

#ifndef D2SET_H
#define D2SET_H

#include <cstdint>

namespace game {

template <typename T>
struct SetNode
{
    int unknown;         /**< Can be zero or one. */
    SetNode<T>* next;    /**< Next node to search. */
    SetNode<T>* less;    /**< Node with lesser value. */
    SetNode<T>* greater; /**< Node with greater value. */
    T value;
};

/** Implementation of std::set<T> used in game. */
template <typename T>
struct Set
{
    char unknown;
    char padding[3];
    std::uint32_t length; /**< Number of nodes in set. */
    char unknown2[4];
    SetNode<T>* begin; /**< Points to the first node with minimal value. */
    int unknown3;
    SetNode<T>* end; /**< Points to the end node used as stop element. */
    void* allocator;
};

template <typename T>
struct SetIterator
{
    char unknown;
    char padding[3];
    SetNode<T>* node;
    SetNode<T>* node2;
};

} // namespace game

#endif // D2SET_H
