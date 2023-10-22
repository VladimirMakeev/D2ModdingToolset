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

#ifndef MIDGARDMAP_H
#define MIDGARDMAP_H

#include "midgardid.h"
#include "midscenarioobject.h"
#include "pointset.h"

namespace game {

struct CVisitorAddPlayer;
struct LTerrainCategory;
struct IMidgardObjectMap;
struct LGroundCategory;

struct CMidgardMap : public IMidScenarioObject
{
    int mapSize;
};

assert_size(CMidgardMap, 12);

namespace CMidgardMapApi {

struct Api
{
    using ChangeTerrain = bool(__thiscall*)(CMidgardMap* thisptr,
                                            CVisitorAddPlayer* visitor,
                                            const LTerrainCategory* terrain,
                                            PointSet* tiles,
                                            PointSet* unknown,
                                            IMidgardObjectMap* objectMap);
    ChangeTerrain changeTerrain;

    /** Returns ground type of map tile at specified position. */
    using GetGround = bool(__thiscall*)(const CMidgardMap* thisptr,
                                        LGroundCategory* ground,
                                        const CMqPoint* position,
                                        const IMidgardObjectMap* objectMap);
    GetGround getGround;
};

Api& get();

} // namespace CMidgardMapApi

} // namespace game

#endif // MIDGARDMAP_H
