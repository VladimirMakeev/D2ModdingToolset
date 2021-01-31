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

#ifndef ITEMPOTIONBOOST_H
#define ITEMPOTIONBOOST_H

#include "itembase.h"
#include "itemexpotionboost.h"
#include <cstddef>

namespace game {

/** Base class for all boost potions. */
struct CItemPotionBoost
    : public IItemExPotionBoost
    , public CItemBase
{
    CMidgardID modPotionId;
};

static_assert(sizeof(CItemPotionBoost) == 20,
              "Size of CItemPotionBoost structure must be exactly 20 bytes");

static_assert(
    offsetof(CItemPotionBoost, CItemPotionBoost::IItemExPotionBoost::vftable) == 0,
    "Vftable offset for IItemExPotionBoost in CItemPotionBoost structure must be 0 bytes");

static_assert(offsetof(CItemPotionBoost, CItemPotionBoost::CItemBase::vftable) == 4,
              "Vftable offset for CItemBase in CItemPotionBoost structure must be 4 bytes");

static_assert(offsetof(CItemPotionBoost, modPotionId) == 16,
              "CItemPotionBoost::modPotionId offset must be 16 bytes");

} // namespace game

#endif // ITEMPOTIONBOOST_H
