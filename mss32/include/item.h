/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#ifndef ITEM_H
#define ITEM_H

#include "d2assert.h"

namespace game {

struct IItemVftable;
struct LItemCategory;
struct Bank;
struct CMidgardID;
struct IItemExtension;
struct GlobalData;

/** Interface for all items read from GItem.dbf. */
struct IItem
{
    IItemVftable* vftable;
};

struct IItemVftable
{
    void* destructor;

    /** Returns corresponding item category. */
    using GetCategory = const LItemCategory*(__thiscall*)(const IItem* thisptr);
    GetCategory getCategory;

    using GetCString = const char*(__thiscall*)(const IItem* thisptr);
    GetCString getName;
    GetCString getDescription;

    /** Returns item cost. */
    using GetValue = const Bank*(__thiscall*)(const IItem* thisptr);
    GetValue getValue;

    using GetUnitId = const CMidgardID*(__thiscall*)(const IItem* thisptr);
    GetUnitId getUnitId;

    /** Functions just like IUsUnit::Cast. However, all items implementations simply return
     * extension interface offset, so it is not clear why this is used instead of dynamic_cast.
     */
    using Cast = IItemExtension*(__thiscall*)(const IItem* thisptr, const char* rawTypeName);
    Cast cast;

    /** Validates item data like modEquipId and attackId.
     * Throws exceptions like "ItemEquipment: Invalid modifier..." if invalid.
     */
    using Validate = void(__thiscall*)(const IItem* thisptr, const GlobalData** globalData);
    Validate validate;
};

assert_vftable_size(IItemVftable, 8);

} // namespace game

#endif // ITEM_H
