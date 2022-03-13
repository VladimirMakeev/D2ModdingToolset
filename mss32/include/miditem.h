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

#ifndef MIDITEM_H
#define MIDITEM_H

#include "midgardid.h"
#include "midscenarioobject.h"

namespace game {

struct Bank;
struct IMidgardObjectMap;

/** Holds item related data in scenario file and game. */
struct CMidItem : public IMidScenarioObject
{
    CMidgardID itemId;
    CMidgardID globalItemId;
};

static_assert(sizeof(CMidItem) == 12, "Size of CMidItem structure must be exactly 12 bytes");

namespace CMidItemApi {

struct Api
{
    /**
     * Computes item selling price according to game rules.
     * Takes into account sell ratio from GVars.dbf and talisman charges count.
     */
    using GetSellingPrice = Bank*(__stdcall*)(Bank* sellingPrice,
                                              const IMidgardObjectMap* objectMap,
                                              const CMidgardID* itemId);
    GetSellingPrice getSellingPrice;
};

Api& get();

} // namespace CMidItemApi

} // namespace game

#endif // MIDITEM_H
