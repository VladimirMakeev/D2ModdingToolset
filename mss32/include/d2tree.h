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

enum class TreeNodeColor : int
{
    Red = 0,
    Black,
};

template <typename T>
struct TreeNode
{
    TreeNodeColor color; /**< Red or Black, Black if head. */
    TreeNode<T>* parent; /**< parent, or root of tree if head. */
    TreeNode<T>* left;   /**< left subtree, or smallest element if head. */
    TreeNode<T>* right;  /**< right subtree, or largest element if head. */
    T value;
};

template <typename T>
struct ConstTreeIterator
{
    char unknown;
    char padding[3];
    TreeNode<T>* node;
    TreeNode<T>* nil;

    bool operator==(const ConstTreeIterator<T>& value) const
    {
        return node == value.node && nil == value.nil;
    }

    bool operator!=(const ConstTreeIterator<T>& value) const
    {
        return !(*this == value);
    }

    const T& operator*() const
    {
        return node->value;
    }

    const T* operator->() const
    {
        return &node->value;
    }

    ConstTreeIterator<T>& operator++()
    {
        if (node->right == nil) {
            for (; node == node->parent->right; node = node->parent)
                ;
            if (node->right != node->parent)
                node = node->parent;
        } else {
            for (node = node->right; node->left != nil; node = node->left)
                ;
        }

        return *this;
    }

    ConstTreeIterator<T> operator++(int)
    {
        auto result = *this;
        ++*this;
        return result;
    }
};

template <typename T>
struct TreeIterator : public ConstTreeIterator<T>
{
    T& operator*() const
    {
        return node->value;
    }

    T* operator->() const
    {
        return &node->value;
    }

    TreeIterator<T>& operator++()
    {
        __super::operator++();
        return *this;
    }

    TreeIterator<T> operator++(int)
    {
        auto result = *this;
        __super::operator++();
        return result;
    }
};

/** Implementation of std::_Tree<T> (ordered red-black tree) used in game. */
template <typename T, typename Allocator>
struct Tree
{
    char unknown;
    char padding[3];
    std::uint32_t length; /**< Number of nodes. */
    char unknown2[4];
    TreeNode<T>* head; /**< Special node that is not a part of the tree. */
    Allocator allocator;
    TreeNode<T>* nil; /**< All NIL leaves point to this node. */
    Allocator allocator2;

    TreeIterator<T> begin()
    {
        return {0, {}, head->left, nil};
    }

    ConstTreeIterator<T> begin() const
    {
        return {0, {}, head->left, nil};
    }

    TreeIterator<T> end()
    {
        return {0, {}, head, nil};
    }

    ConstTreeIterator<T> end() const
    {
        return {0, {}, head, nil};
    }
};

} // namespace game

#endif // D2TREE_H
