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

#ifndef ITEMEQUIPMENT_H
#define ITEMEQUIPMENT_H

#include "itembase.h"
#include "itemexequipment.h"
#include <cstddef>

namespace game {

/** Base class for all items that stack leader can equip. */
struct CItemEquipment
    : public IItemExEquipment
    , public CItemBase
{
    CMidgardID modEquipId;
    CMidgardID attackId;
};

static_assert(sizeof(CItemEquipment) == 24,
              "Size of CItemEquipment structure must be exactly 24 bytes");

static_assert(offsetof(CItemEquipment, CItemEquipment::IItemExEquipment::vftable) == 0,
              "Vftable offset for IItemExEquipment in CItemEquipment structure must be 0 bytes");

static_assert(offsetof(CItemEquipment, CItemEquipment::CItemBase::vftable) == 4,
              "Vftable offset for CItemBase in CItemEquipment structure must be 4 bytes");

static_assert(offsetof(CItemEquipment, modEquipId) == 16,
              "CItemEquipment::modEquipId offset must be 16 bytes");

} // namespace game

#endif // ITEMEQUIPMENT_H
