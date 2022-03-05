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

#ifndef IDLIST_H
#define IDLIST_H

#include "d2list.h"
#include "midgardid.h"

namespace game {

using IdList = List<CMidgardID>;
using IdListNode = ListNode<CMidgardID>;
using IdListIterator = ListIterator<CMidgardID>;

namespace IdListApi {

struct Api
{
    using Constructor = IdList*(__thiscall*)(IdList* thisptr);
    Constructor constructor;

    using Destructor = void(__thiscall*)(IdList* thisptr);
    Destructor destructor;

    /** Clears list contents. */
    using Clear = bool(__thiscall*)(IdList* thisptr);
    Clear clear;

    /** Adds id to list's end. */
    using PushBack = int(__thiscall*)(IdList* thisptr, const CMidgardID* id);
    PushBack pushBack;

    /** Erases id from list. */
    using Erase = void(__thiscall*)(IdList* thisptr, IdListIterator pos);
    Erase erase;

    using Front = CMidgardID*(__thiscall*)(const IdList* thisptr);
    Front front;

    using GetIterator = IdListIterator*(__thiscall*)(const IdList* thisptr, IdListIterator* value);
    GetIterator begin;
    GetIterator end;

    using Find = IdListIterator*(__stdcall*)(IdListIterator* value,
                                             IdListIterator begin,
                                             IdListIterator end,
                                             const CMidgardID* id);
    Find find;

    using Equals = bool(__thiscall*)(IdListIterator* thisptr, const IdListIterator* value);
    Equals equals;

    using Dereference = CMidgardID*(__thiscall*)(IdListIterator* thisptr);
    Dereference dereference;

    using Preincrement = IdListIterator*(__thiscall*)(IdListIterator* thisptr);
    Preincrement preinc;

    using Shuffle = void(__stdcall*)(IdList* list);
    Shuffle shuffle;
};

Api& get();

} // namespace IdListApi

} // namespace game

#endif // IDLIST_H
