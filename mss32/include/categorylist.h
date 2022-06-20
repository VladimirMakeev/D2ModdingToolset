/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#ifndef CATEGORYLIST_H
#define CATEGORYLIST_H

#include "categories.h"
#include "d2list.h"

namespace game {

using CategoryList = List<Category<int>>;
using CategoryListNode = ListNode<Category<int>>;

namespace CategoryListApi {

struct Api
{
    using Constructor = CategoryList*(__thiscall*)(CategoryList* thisptr);
    Constructor constructor;

    using Clear = void(__thiscall*)(CategoryList* thisptr);
    Clear clear;

    using FreeNode = void(__thiscall*)(CategoryList* thisptr, CategoryListNode* node);
    FreeNode freeNode;
};

Api& get();

} // namespace CategoryListApi

} // namespace game

#endif // CATEGORYLIST_H
