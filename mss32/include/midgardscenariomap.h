/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
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

#ifndef MIDGARDSCENARIOMAP_H
#define MIDGARDSCENARIOMAP_H

#include "midgardobjectmap.h"

namespace game {

struct CMidgardScenarioMap : public IMidgardObjectMap
{
    char unknown[364];
};

static_assert(sizeof(CMidgardScenarioMap) == 368,
              "Size of CMidgardScenarioMap structure must be exactly 368 bytes");

} // namespace game

#endif // MIDGARDSCENARIOMAP_H
