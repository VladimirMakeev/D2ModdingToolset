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

struct CMidgardMap : public IMidScenarioObject
{
    CMidgardID mapId;
    int mapSize;
};

static_assert(sizeof(CMidgardMap) == 12, "Size of CMidgardMap structure must be exactly 12 bytes");

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
};

Api& get();

} // namespace CMidgardMapApi

} // namespace game

#endif // MIDGARDMAP_H
