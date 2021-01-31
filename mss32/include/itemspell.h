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

#ifndef ITEMSPELL_H
#define ITEMSPELL_H

#include "itembase.h"
#include "itemexspell.h"
#include <cstddef>

namespace game {

/** Base class for all spell related items. */
struct CItemSpell
    : public IItemExSpell
    , public CItemBase
{
    CMidgardID spellId;
};

static_assert(sizeof(CItemSpell) == 20, "Size of CItemSpell structure must be exactly 20 bytes");

static_assert(offsetof(CItemSpell, CItemSpell::IItemExSpell::vftable) == 0,
              "Vftable offset for IItemExSpell in CItemSpell structure must be 0 bytes");

static_assert(offsetof(CItemSpell, CItemSpell::CItemBase::vftable) == 4,
              "Vftable offset for CItemBase in CItemSpell structure must be 4 bytes");

static_assert(offsetof(CItemSpell, spellId) == 16, "CItemSpell::spellId offset must be 16 bytes");

} // namespace game

#endif // ITEMSPELL_H
