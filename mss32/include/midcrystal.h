/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef MIDCRYSTAL_H
#define MIDCRYSTAL_H

#include "aipriority.h"
#include "mapelement.h"
#include "midscenarioobject.h"
#include "resourcetype.h"

namespace game {

/** Represents gold mine or mana crystal in scenario file and game. */
struct CMidCrystal : public IMidScenarioObject
{
    IMapElement mapElement;
    IAiPriority aiPriority;
    LResourceType resourceType;
};

assert_size(CMidCrystal, 48);

} // namespace game

#endif // MIDCRYSTAL_H
