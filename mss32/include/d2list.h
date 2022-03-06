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

/** Implementation of std::list<T> used in game. */
template <typename T>
struct List
{
    std::uint32_t length;
    ListNode<T>* head;
    int unknown;
    void* allocator;
};

template <typename T>
struct ListIterator
{
    char unknown;
    char padding[3];
    ListNode<T>* node;
    ListNode<T>* node2;
};

} // namespace game

#endif // D2LIST_H
