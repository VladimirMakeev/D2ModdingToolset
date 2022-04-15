/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef UNITPOSITIONPAIR_H
#define UNITPOSITIONPAIR_H

#include "d2pair.h"

namespace game {

/**
 * Unit position (0-5) to mark during single battle turn.
 * The second component determines if the position is in attack's target list
 * (IBatAttack::FillTargetsList; position has frame highlight on group panel).
 */
using UnitPositionPair = Pair<int, bool>;

static_assert(sizeof(UnitPositionPair) == 8,
              "Size of UnitPositionPair structure must be exactly 8 bytes");

} // namespace game

#endif // UNITPOSITIONPAIR_H
