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

#ifndef ITEMPOTIONBOOSTTEMP_H
#define ITEMPOTIONBOOSTTEMP_H

#include "itempotionboost.h"

namespace game {

/** Represents temporary (single day) boost potion. */
struct CItemPotionBoostTemp : public CItemPotionBoost
{ };

assert_size(CItemPotionBoostTemp, 20);

} // namespace game

#endif // ITEMPOTIONBOOSTTEMP_H
