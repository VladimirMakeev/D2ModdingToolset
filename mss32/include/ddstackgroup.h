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

#ifndef DDSTACKGROUP_H
#define DDSTACKGROUP_H

#include "ddunitgroup.h"

namespace game {

struct ITask;
struct CPhaseGame;
struct CMidDragDropInterf;
struct IResetStackExt;

struct CDDStackGroupData
{
    ITask* task;
    CPhaseGame* phaseGame;
    int unknown;
    CMidDragDropInterf* dragDropInterf;
    IResetStackExt* resetStackExt;
    IMidgardObjectMap* objectMap;
    CMidgardID id1;
    CMidgardID id2;
    char unknown3[4];
    SmartPtr<IMqImage2> defaultCursorImage;
    SmartPtr<IMqImage2> noDragDropCursorImage;
};

assert_size(CDDStackGroupData, 52);

struct CDDStackGroup : public CDDUnitGroup
{
    CDDStackGroupData* data;
};

assert_size(CDDStackGroup, 20);

} // namespace game

#endif // DDSTACKGROUP_H
