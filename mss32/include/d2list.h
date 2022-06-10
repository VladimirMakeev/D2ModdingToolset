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

#ifndef D2LIST_H
#define D2LIST_H

#include "d2assert.h"
#include <cstdint>

namespace game {

/** Implementation of std::list<T>::node used in game. */
template <typename T>
struct ListNode
{
    ListNode<T>* next;
    ListNode<T>* prev;
    T data;
};

assert_size(ListNode<int>, 12);

/** Implementation of std::list<T>::const_iterator used in game.
 * Inline methods exactly correspond to in-game implementation.
 * T* ptr appear to always be nullptr, but no guarantees.
 * Bet that this implementation might be shared with vector::iterator. */
template <typename T>
struct ConstListIterator
{
    char unknown;
    char padding[3];
    ListNode<T>* node;
    T* ptr;

    bool operator==(const ConstListIterator<T>& value) const
    {
        return ptr ? ptr == value.ptr : node == value.node;
    }

    bool operator!=(const ConstListIterator<T>& value) const
    {
        return !(*this == value);
    }

    const T& operator*() const
    {
        return ptr ? *ptr : node->data;
    }

    const T* operator->() const
    {
        return ptr ? ptr : &node->data;
    }

    ConstListIterator<T>& operator++()
    {
        if (ptr)
            ptr++;
        else
            node = node->next;

        return *this;
    }

    ConstListIterator<T> operator++(int)
    {
        auto result = *this;
        ++*this;
        return result;
    }
};

assert_size(ConstListIterator<int>, 12);

/** Implementation of std::list<T>::iterator used in game.
 * Inline methods exactly correspond to in-game implementation. */
template <typename T>
struct ListIterator : public ConstListIterator<T>
{
    using Base = ConstListIterator<T>;

    T& operator*() const
    {
        return ptr ? *ptr : node->data;
    }

    T* operator->() const
    {
        return ptr ? ptr : &node->data;
    }

    ListIterator<T>& operator++()
    {
        Base::operator++();
        return *this;
    }

    ListIterator<T> operator++(int)
    {
        auto result = *this;
        Base::operator++();
        return result;
    }
};

assert_size(ListIterator<int>, 12);

/** Implementation of std::list<T> used in game.
 * Inline methods exactly correspond to in-game implementation. */
template <typename T>
struct List
{
    std::uint32_t length;
    ListNode<T>* head;
    int unknown;
    void* allocator;

    ListIterator<T> begin()
    {
        return {0, {}, head->next, nullptr};
    }

    ConstListIterator<T> begin() const
    {
        return {0, {}, head->next, nullptr};
    }

    ListIterator<T> end()
    {
        return {0, {}, head, nullptr};
    }

    ConstListIterator<T> end() const
    {
        return {0, {}, head, nullptr};
    }
};

assert_size(List<int>, 16);

} // namespace game

#endif // D2LIST_H
