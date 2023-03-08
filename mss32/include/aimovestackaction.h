/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#ifndef AIMOVESTACKACTION_H
#define AIMOVESTACKACTION_H

#include "aitacticactionsimple.h"
#include "categorylist.h"

namespace game {

struct CAiMoveStackAction : CAiTacticActionSimple
{
    int unknown;
    int unknown4;
    int unknown8;
    int unknown12;
    int unknown16;
    int unknown20;
    int unknown24;
    int unknown28;
    int unknown32;
    CategoryList unknown36;
    int unknown52;
    bool unknown56;
    bool unknown57;
    bool unknown58;
    char padding;
};

assert_size(CAiMoveStackAction, 84);
assert_offset(CAiMoveStackAction, unknown36, sizeof(CAiTacticActionSimple) + 36);

namespace CAiMoveStackActionApi {

struct Vftable
{
    const IAiTacticActionVftable* action;
    const IAiReactionVftable* reaction;
};

const Vftable& vftable();

} // namespace CAiMoveStackActionApi

} // namespace game

#endif // AIMOVESTACKACTION_H
