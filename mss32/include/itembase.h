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

#ifndef ITEMBASE_H
#define ITEMBASE_H

#include "currency.h"
#include "item.h"
#include "midgardid.h"
#include "textandid.h"

namespace game {

struct CItemBaseData
{
    TextAndId name;
    TextAndId description;
    Bank value;
    CMidgardID unitId;
};

assert_size(CItemBaseData, 32);

/** Base class for all items read from GItem.dbf. */
struct CItemBase : public IItem
{
    CMidgardID itemId;
    CItemBaseData* data;
};

assert_size(CItemBase, 12);

} // namespace game

#endif // ITEMBASE_H
