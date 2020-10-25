/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#ifndef FORTIFICATION_H
#define FORTIFICATION_H

#include "aipriority.h"
#include "mapelement.h"
#include "midinventory.h"
#include "midscenarioobject.h"
#include "midunitgroup.h"
#include "stringandid.h"
#include <cstddef>

namespace game {

/** Base class for capitals and villages (cities). */
struct CFortification : public IMidScenarioObject
{
    CMidgardID cityId;
    IMapElement mapElement;
    IAiPriority aiPriority;
    StringAndId name;
    StringAndId description;
    CMidUnitGroupFortification group;
    CMidInventory inventory;
    CMidgardID ownerId;
    CMidgardID subraceId;
    CMidgardID stackId;
};

static_assert(offsetof(CFortification, aiPriority) == 28,
              "CFortification::aiPriority offset must be 28 bytes");

static_assert(offsetof(CFortification, name) == 36, "CFortification::name offset must be 36 bytes");

static_assert(offsetof(CFortification, description) == 48,
              "CFortification::description offset must be 48 bytes");

static_assert(offsetof(CFortification, group) == 60,
              "CFortification::group offset must be 60 bytes");

static_assert(offsetof(CFortification, inventory) == 112,
              "CFortification::inventory offset must be 112 bytes");

} // namespace game

#endif // FORTIFICATION_H
