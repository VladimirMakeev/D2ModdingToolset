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

#ifndef DDUNITGROUP_H
#define DDUNITGROUP_H

#include "idvector.h"
#include "middropmanager.h"
#include "middropsource.h"
#include "middroptarget.h"
#include "midunitext.h"

namespace game {

struct CMidUnitGroupAdapter;
struct CGroupAreaManagement;
struct IMidgardObjectMap;

struct CDDUnitGroupData
{
    int unknown;
    int unknown2;
    int unknown3;
    CMidgardID groupId;
    CMidUnitGroupAdapter* unitGroupAdapter;
    CMidgardID id2;
    IMidgardObjectMap* objectMap;
    IMidDropManager* dropManager;
    int unknown7;
    int unknown8;
    int unknown9;
    int unknown10;
    IdVector idArray;
    SmartPtr<CursorHandle> defaultCursorHandle;
    SmartPtr<CursorHandle> noDragDropCursorHandle;
    SmartPointer ptr;
    IMqImage2* dragDropValidSmall;
    IMqImage2* dragDropValidLarge;
    IMqImage2* dragDropInvalidSmall;
    IMqImage2* dragDropInvalidLarge;
    int array[6];
    char unknown16[12];
    SmartPtr<IMqImage2> spellBoostIconPtr;
    SmartPtr<IMqImage2> spellLowerIconPtr;
    SmartPtr<IMqImage2> potionBoostIconPtr;
    SmartPtr<IMqImage2> upgrageIconPtr;
    SmartPtr<IMqImage2> highLevelIcons[3];
    CGroupAreaManagement* areaManagement;
};

assert_size(CDDUnitGroupData, 200);

/** Implements drag and drop logic for unit group. */
struct CDDUnitGroup
    : public IMidDropSource
    , public IMidDropTarget
    , public IMidUnitExt
{
    CDDUnitGroupData* unitGroupData;
};

assert_size(CDDUnitGroup, 16);

} // namespace game

#endif // DDUNITGROUP_H
