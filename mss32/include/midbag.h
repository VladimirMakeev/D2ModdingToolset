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

#ifndef MIDBAG_H
#define MIDBAG_H

#include "aipriority.h"
#include "mapelement.h"
#include "midgardid.h"
#include "midinventory.h"
#include "midscenarioobject.h"
#include <cstddef>

namespace game {

/** Represents treasure chests in scenario file and game. */
struct CMidBag : public IMidScenarioObject
{
    IMapElement mapElement;
    IAiPriority aiPriority;
    CMidInventory inventory;
    int image;
};

static_assert(sizeof(CMidBag) == 60, "Size of CMidBag structure must be exactly 60 bytes");

static_assert(offsetof(CMidBag, mapElement) == 8, "CMidBag::mapElement offset must be 8 bytes");

static_assert(offsetof(CMidBag, aiPriority) == 28, "CMidBag::aiPriority offset must be 28 bytes");

static_assert(offsetof(CMidBag, inventory) == 36, "CMidBag::inventory offset must be 36 bytes");

} // namespace game

#endif // MIDBAG_H
