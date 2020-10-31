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

#ifndef MIDRUIN_H
#define MIDRUIN_H

#include "aipriority.h"
#include "currency.h"
#include "mapelement.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "midunitgroup.h"
#include "stringandid.h"
#include <cstddef>

namespace game {

struct CMidUnitGroupRuin : public CMidUnitGroup
{ };

struct CMidRuin : public IMidScenarioObject
{
    CMidgardID ruinId;
    IMapElement mapElement;
    IAiPriority aiPriority;
    CMidUnitGroupRuin group;
    StringAndId title;
    StringAndId description;
    Bank cash;
    CMidgardID itemId;
    CMidgardID looterId;
    char visitorData[28];
    int imageIndex;
};

static_assert(sizeof(CMidRuin) == 164, "Size of CMidRuin structure must be exactly 164 bytes");

static_assert(offsetof(CMidRuin, group) == 36, "CMidRuin::group offset must be 36 bytes");

static_assert(offsetof(CMidRuin, description) == 100,
              "CMidRuin::description offset must be 100 bytes");

static_assert(offsetof(CMidRuin, looterId) == 128, "CMidRuin::looterId offset must be 128 bytes");

} // namespace game

#endif // MIDRUIN_H
