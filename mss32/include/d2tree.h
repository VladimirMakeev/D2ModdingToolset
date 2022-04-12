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

#ifndef D2TREE_H
#define D2TREE_H

#include <cstdint>

namespace game {

template <typename T, typename Node>
struct TreeNode
{
    int unknown;   /**< Can be zero or one. */
    Node* next;    /**< Next node to search. */
    Node* less;    /**< Node with lesser value. */
    Node* greater; /**< Node with greater value. */
    T value;
};

/** Implementation of std::_Tree<T> (ordered red-black tree) used in game. */
template <typename T, typename Node, typename Allocator>
struct Tree
{
    char unknown;
    char padding[3];
    std::uint32_t length; /**< Number of nodes. */
    char unknown2[4];
    Node* begin; /**< Points to the first node with minimal value. */
    Allocator allocator;
    Node* end; /**< Points to the end node used as stop element. */
    Allocator allocator2;
};

template <typename T, typename Node>
struct TreeIterator
{
    char unknown;
    char padding[3];
    Node* node;
    Node* node2;
};

} // namespace game

#endif // D2TREE_H
