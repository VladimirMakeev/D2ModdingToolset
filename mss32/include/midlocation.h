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

#ifndef MIDLOCATION_H
#define MIDLOCATION_H

#include "mapelement.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "stringandid.h"
#include <cstddef>

namespace game {

/** Holds location related data in scenario file and game. */
struct CMidLocation
    : public IMapElement
    , public IMidScenarioObject
{
    StringAndId name;
    int radius;
};

assert_size(CMidLocation, 44);
assert_offset(CMidLocation, CMidLocation::IMapElement::vftable, 0);
assert_offset(CMidLocation, CMidLocation::IMidScenarioObject::vftable, 20);

} // namespace game

#endif // MIDLOCATION_H
