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

#ifndef MIDUNITGROUPADAPTER_H
#define MIDUNITGROUPADAPTER_H

#include "idvector.h"
#include "smartptr.h"
#include "unitgroup.h"

namespace game {

struct IMidgardObjectMap;

struct GroupAdapterUnitData
{
    bool unknown;
    bool unknown2;
    bool unknown3;
    char padding;
    int strengthTier;
    bool boostApplied;
    bool lowerApplied;
    bool potionApplied;
    bool canLevelUp;
};

assert_size(GroupAdapterUnitData, 12);

struct CMidUnitGroupAdapterData
{
    IMidgardObjectMap* objectMap;
    Vector<SmartPointer> array;
    Vector<SmartPointer> array2;
    IdVector idArray;
    int unknown2;
    CMidgardID groupId;
    CMidgardID playerId;
    GroupAdapterUnitData unitData[6];
    char unknown3[8];
};

assert_size(CMidUnitGroupAdapterData, 144);

struct CMidUnitGroupAdapter : public IUnitGroup
{
    CMidUnitGroupAdapterData* data;
};

assert_size(CMidUnitGroupAdapter, 8);

} // namespace game

#endif // MIDUNITGROUPADAPTER_H
