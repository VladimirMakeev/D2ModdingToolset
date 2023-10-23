/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#ifndef MIDGARDMAPFOG_H
#define MIDGARDMAPFOG_H

#include "d2pair.h"
#include "midscenarioobject.h"
#include "mqpoint.h"
#include <cstddef>
#include <cstdint>

namespace game {

/** Holds player's fog of war related data in scenario file and game. */
struct CMidgardMapFog : public IMidScenarioObject
{
    std::uint32_t mapSize;
    Pair<std::size_t, void*> fogCells;
};

assert_size(CMidgardMapFog, 20);

namespace CMidgardMapFogApi {

struct Api
{
    /**
     * Returns fog of war at specified map position.
     * @param[in] thisptr map fog object
     * @param[out] fog pointer where to store fog result
     * @param[in] mapPosition position of a tile to check fog at
     * @returns true on success. It means fog pointer contains actual data
     */
    using GetFog = bool(__thiscall*)(const CMidgardMapFog* thisptr,
                                     bool* fog,
                                     const CMqPoint* mapPosition);

    GetFog getFog;
};

Api& get();

} // namespace CMidgardMapFogApi

} // namespace game

#endif // MIDGARDMAPFOG_H
