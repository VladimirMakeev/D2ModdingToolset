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

#ifndef ITEMBATTLE_H
#define ITEMBATTLE_H

#include "itembase.h"
#include "itemexequipment.h"
#include <cstddef>

namespace game {

/** Base class for all non-potion type items that stack leader can use in battle. */
struct CItemBattle
    : public IItemExEquipment
    , public CItemBase
{
    CMidgardID attackId;
    CMidgardID modEquipId;
};

static_assert(sizeof(CItemBattle) == 24, "Size of CItemBattle structure must be exactly 24 bytes");

static_assert(offsetof(CItemBattle, CItemBattle::IItemExEquipment::vftable) == 0,
              "Vftable offset for IItemExEquipment in CItemBattle structure must be 0 bytes");

static_assert(offsetof(CItemBattle, CItemBattle::CItemBase::vftable) == 4,
              "Vftable offset for CItemBase in CItemBattle structure must be 4 bytes");

static_assert(offsetof(CItemBattle, attackId) == 16,
              "CItemBattle::attackId offset must be 16 bytes");

} // namespace game

#endif // ITEMBATTLE_H
