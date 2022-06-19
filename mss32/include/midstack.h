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

/** Index map for CMidStack::leaderEquippedItems */
enum EquippedItemIdx
{
    Banner = 0,
    Tome,
    Battle1,
    Battle2,
    Artifact1,
    Artifact2,
    Boots,
};

/**
 * Holds stack related data in scenario file and game.
 * Stack is a group of 6 units, one of which is a leader.
 */
struct CMidStack
    : public IMapElement
    , public IMidScenarioObject
{
    IAiPriority aiPriority;
    CMidgardID sourceTemplateId;
    CMidgardID leaderId;
    bool leaderAlive;
    char padding[3];
    CMidUnitGroupStack group;
    CMidInventoryStack inventory;
    IdVector leaderEquippedItems;
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

assert_size(CMidStack, 200);
assert_offset(CMidStack, CMidStack::IMapElement::vftable, 0);
assert_offset(CMidStack, CMidStack::IMidScenarioObject::vftable, 20);
assert_offset(CMidStack, id, 24);
assert_offset(CMidStack, leaderId, 40);
assert_offset(CMidStack, inventory, 100);
assert_offset(CMidStack, leaderEquippedItems, 124);
assert_offset(CMidStack, orderTargetId, 172);

namespace CMidStackApi {

struct Api
{
    using SetPosition = bool(__thiscall*)(CMidStack* thisptr,
                                          const IMidgardObjectMap* objectMap,
                                          const CMqPoint* position,
                                          bool checkArea);
    SetPosition setPosition;

    using SetOwner = bool(__thiscall*)(CMidStack* thisptr,
                                       const IMidgardObjectMap* objectMap,
                                       const CMidgardID* ownerId,
                                       const CMidgardID* subraceId);
    SetOwner setOwner;
};

Api& get();

const IMapElementVftable* vftable();

} // namespace CMidStackApi

} // namespace game

#endif // MIDSTACK_H
