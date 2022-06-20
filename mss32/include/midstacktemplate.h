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

#ifndef MIDSTACKTEMPLATE_H
#define MIDSTACKTEMPLATE_H

#include "d2list.h"
#include "idvector.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "ordercat.h"
#include "stringandid.h"
#include <cstddef>

namespace game {

enum class Facing : std::uint8_t
{
    SouthWest,
    West,
    NorthWest,
    North,
    NorthEast,
    East,
    SouthEast,
    South
};

struct StackTemplateListNode
{
    char unknown[8];
};

struct CMidStackTemplate : public IMidScenarioObject
{
    CMidgardID ownerId;
    CMidgardID subraceId;
    LOrderCategory orderCategory;
    CMidgardID orderTargetId;
    StringAndId name;
    CMidgardID leaderId;
    IdVector units;
    CMidgardID positions[6];
    bool useFacing;
    Facing facing;
    char padding[2];
    List<StackTemplateListNode> modifiers;
    int aiPriority;
};

assert_size(CMidStackTemplate, 112);
assert_offset(CMidStackTemplate, orderTargetId, 28);
assert_offset(CMidStackTemplate, facing, 89);

} // namespace game

#endif // MIDSTACKTEMPLATE_H
