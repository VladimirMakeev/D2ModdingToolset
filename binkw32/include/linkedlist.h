/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "midgardid.h"
#include <cstdint>

namespace game {

struct IdListNode
{
    IdListNode* next;
    IdListNode* prev;
    CMidgardID id;
};

/**
 * Doubly-linked list with CMidgardID as data.
 */
struct IdList
{
    std::uint32_t length;
    IdListNode* head;
    int unknown;
    void* allocator;
};

namespace IdListApi {

struct Api
{
    /** Clears list contents. */
    using SetEmpty = bool(__thiscall*)(IdList* list);
    SetEmpty setEmpty;

    /** Adds id to list. */
    using Add = int(__thiscall*)(IdList* list, const CMidgardID* id);
    Add add;
};

Api& get();

} // namespace IdListApi

} // namespace game

#endif // LINKEDLIST_H
