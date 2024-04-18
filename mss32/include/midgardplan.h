/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef MIDGARDPLAN_H
#define MIDGARDPLAN_H

#include "d2vector.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "mqpoint.h"
#include <cstdint>

namespace game {

struct MidgardPlanElement
{
    std::uint16_t y;
    std::uint16_t x;
    CMidgardID elementId;
};

assert_size(MidgardPlanElement, 8);

using PlanElements = Vector<MidgardPlanElement>;

/**
 * Element flags packing:
 *
 * +--+--+--+--+--+--+--+--+
 * |S3|D3|S2|D2|S1|D1|S0|D0|
 * +--+--+--+--+--+--+--+--+
 *   7  6  5  4  3  2  1  0
 *
 * DN - bit indicating presence of a dynamic element on a tile with relative X position of N.
 * SN - bit indicating presence of a static element on a tile with relative X position of N.
 *
 * Relative X position: (X & 3)
 */

/** Utility object used for mapping IMapElements coordinates to scenario object ids. */
struct CMidgardPlan : public IMidScenarioObject
{
    CMidgardID unknownId;
    int mapSize;
    /**
     * 36 x 144 array of bit flags indicating presence of a static or dynamic elements on a tile.
     * Each array element stores flags of 4 adjacent tiles along X axis.
     * Accessed as: 36 * Y + X / 4.
     */
    std::uint8_t elementFlags[5184];
    /** Static map objects such as: Fort, Road, Landmark, Site, Ruin, Crystal, Location. */
    PlanElements staticElements;
    /** Dynamic objects: Stack, Bag, Tomb, Rod. */
    PlanElements dynamicElements;
};

assert_size(CMidgardPlan, 5232);

namespace CMidgardPlanApi {

struct Api
{
    /**
     * Searches for map object with specified type at position.
     * @returns id of found object or nullptr if nothing found.
     */
    using GetObjectId = const CMidgardID*(__thiscall*)(const CMidgardPlan* thisptr,
                                                       const CMqPoint* mapPosition,
                                                       const IdType* type);
    GetObjectId getObjectId;

    /** Returns true if plan contains object of one of the specified types at position. */
    using IsPositionContainsObjects = bool(__thiscall*)(const CMidgardPlan* thisptr,
                                                        const CMqPoint* mapPosition,
                                                        const IdType* objectTypes,
                                                        std::uint32_t typesTotal);
    IsPositionContainsObjects isPositionContainsObjects;
};

Api& get();

} // namespace CMidgardPlanApi

} // namespace game

#endif // MIDGARDPLAN_H
