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

#include <cstdint>

namespace game {

template <typename T>
struct ListNode
{
    ListNode<T>* next;
    ListNode<T>* prev;
    T data;
};

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

template <typename T>
struct ListIterator : public ConstListIterator<T>
{
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
        __super::operator++();
        return *this;
    }

    ListIterator<T> operator++(int)
    {
        auto result = *this;
        __super::operator++();
        return result;
    }
};

/** Implementation of std::list<T> used in game. */
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

} // namespace game

#endif // D2LIST_H
