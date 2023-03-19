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

#ifndef AITACTICACTIONSIMPLE_H
#define AITACTICACTIONSIMPLE_H

#include "aireactionnull.h"
#include "aitacticaction.h"
#include "d2assert.h"
#include "midgardid.h"
#include "mqpoint.h"

namespace game {

struct CAiTacticActionSimple
    : IAiTacticAction
    , CAiReactionNull
{
    CMidgardID stackId;
    CMidgardID targetId;
    CMqPoint targetPosition;
};

assert_size(CAiTacticActionSimple, 24);

} // namespace game

#endif // AITACTICACTIONSIMPLE_H
