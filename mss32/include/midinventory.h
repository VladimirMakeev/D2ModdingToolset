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

#ifndef MIDINVENTORY_H
#define MIDINVENTORY_H

#include "idvector.h"
#include "midgardid.h"

namespace game {

struct CMidInventoryVftable;
struct IMidgardObjectMap;

struct CMidInventory
{
    CMidInventoryVftable* vftable;
    IdVector items;
};

struct CMidInventoryVftable
{
    void* destructor;

    using Method1 = bool(__thiscall*)(CMidInventory* thisptr,
                                      const CMidgardID* groupId,
                                      const IMidgardObjectMap* objectMap);
    Method1 method1;

    /** Runs CVisitorDestroyItem for each item in inventory. */
    using Clear = bool(__thiscall*)(CMidInventory* thisptr, int* a2, IMidgardObjectMap* objectMap);
    Clear clear;

    /** Serializes inventory data. */
    using Stream = void*(__thiscall*)(CMidInventory* thisptr, int a2, void* a3);
    Stream stream;

    /** Returns true if all item ids refers to valid objects. */
    using AllItemsExist = bool(__thiscall*)(CMidInventory* thisptr, IMidgardObjectMap* objectMap);
    AllItemsExist allItemsExist;

    /** Returns number of items in inventory. */
    using GetItemsCount = int(__thiscall*)(const CMidInventory* thisptr);
    GetItemsCount getItemsCount;

    /** Returns item id at specified index. */
    using GetItem = const CMidgardID*(__thiscall*)(const CMidInventory* thisptr, int index);
    GetItem getItem;

    /** Returns index of specified item id or -1 if item not found. */
    using GetItemIndex = int(__thiscall*)(const CMidInventory* thisptr, const CMidgardID* itemId);
    GetItemIndex getItemIndex;

    /** Adds item id to inventory. Returns true if added successfully. */
    using AddItem = bool(__thiscall*)(CMidInventory* thisptr,
                                      int a2,
                                      const CMidgardID* itemId,
                                      IMidgardObjectMap* objectMap);
    AddItem addItem;

    /** Removes item id from inventory. Returns true if removed successfully. */
    using RemoveItem = bool(__thiscall*)(CMidInventory* thisptr,
                                         int a2,
                                         const CMidgardID* itemId,
                                         IMidgardObjectMap* objectMap);
    RemoveItem removeItem;
};

assert_vftable_size(CMidInventoryVftable, 10);

} // namespace game

#endif // MIDINVENTORY_H
