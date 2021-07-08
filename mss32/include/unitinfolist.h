/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef UNITINFOLIST_H
#define UNITINFOLIST_H

#include "linkedlist.h"

namespace game {

struct UnitInfo;

using UnitInfoList = LinkedList<UnitInfo>;
using UnitInfoListNode = LinkedListNode<UnitInfo>;
using UnitInfoListIterator = LinkedListIterator<UnitInfo>;

namespace UnitInfoListApi {

struct Api
{
    using Constructor = UnitInfoList*(__thiscall*)(UnitInfoList* thisptr);
    Constructor constructor;

    using Destructor = void(__thiscall*)(UnitInfoList* thisptr);
    Destructor destructor;

    using Sort = void(__thiscall*)(UnitInfoList* thisptr);
    Sort sort;

    using GetIterator = UnitInfoListIterator*(__thiscall*)(UnitInfoList* thisptr,
                                                           UnitInfoListIterator* iterator);
    GetIterator begin;
    GetIterator end;

    using Dereference = UnitInfo*(__thiscall*)(UnitInfoListIterator* thisptr);
    Dereference dereference;

    using Equals = bool(__thiscall*)(UnitInfoListIterator* thisptr,
                                     const UnitInfoListIterator* value);
    Equals equals;

    using Preincrement = UnitInfoListIterator*(__thiscall*)(UnitInfoListIterator* thisptr);
    Preincrement preinc;
};

Api& get();

} // namespace UnitInfoListApi

} // namespace game

#endif // UNITINFOLIST_H
