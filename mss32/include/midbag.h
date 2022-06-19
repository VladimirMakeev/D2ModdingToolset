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

assert_size(CMidBag, 60);
assert_offset(CMidBag, mapElement, 8);
assert_offset(CMidBag, aiPriority, 28);
assert_offset(CMidBag, inventory, 36);

} // namespace game

#endif // MIDBAG_H
