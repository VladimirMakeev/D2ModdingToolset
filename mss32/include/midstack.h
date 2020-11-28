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

#ifndef MIDSTACK_H
#define MIDSTACK_H

#include "aipriority.h"
#include "idvector.h"
#include "mapelement.h"
#include "midinventory.h"
#include "midscenarioobject.h"
#include "midunitgroup.h"
#include "ordercat.h"
#include <cstddef>
#include <cstdint>

namespace game {

struct CMidUnitGroupStack : public CMidUnitGroup
{ };

struct CMidInventoryStack : public CMidInventory
{
    int unknown;
};

/**
 * Holds stack related data in scenario file and game.
 * Stack is a group of 6 units, one of which is a leader.
 */
struct CMidStack
    : public IMapElement
    , public IMidScenarioObject
{
    CMidgardID stackId;
    IAiPriority aiPriority;
    CMidgardID sourceTemplateId;
    CMidgardID leaderId;
    bool leaderAlive;
    char padding[3];
    CMidUnitGroupStack group;
    CMidInventoryStack inventory;
    IdVector leaderEquppedItems;
    CMidgardID ownerId;
    CMidgardID subraceId;
    CMidgardID insideId;
    std::uint8_t movement;
    std::uint8_t facing;
    std::uint8_t upgCount;
    bool invisible;
    bool aiIgnore;
    char padding2[3];
    LOrderCategory order;
    CMidgardID orderTargetId;
    LOrderCategory aiOrder;
    CMidgardID aiOrderTargetId;
    int creatLvl;
    int nbBattle;
};

static_assert(sizeof(CMidStack) == 200, "Size of CMidStack structure must be exactly 200 bytes");

static_assert(offsetof(CMidStack, CMidStack::IMapElement::vftable) == 0,
              "Vftable offset for IMapElement in CMidStack structure must be 0 bytes");

static_assert(offsetof(CMidStack, CMidStack::IMidScenarioObject::vftable) == 20,
              "Vftable offset for IMidScenarioObject in CMidStack structure must be 20 bytes");

static_assert(offsetof(CMidStack, stackId) == 24, "CMidStack::stackId offset must be 24 bytes");

static_assert(offsetof(CMidStack, leaderId) == 40, "CMidStack::leaderId offset must be 40 bytes");

static_assert(offsetof(CMidStack, leaderEquppedItems) == 124,
              "CMidStack::leaderEquppedItems offset must be 124 bytes");

static_assert(offsetof(CMidStack, orderTargetId) == 172,
              "CMidStack::orderTargetId offset must be 172 bytes");

} // namespace game

#endif // MIDSTACK_H
