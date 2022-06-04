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

#ifndef TASKMAPCHANGE_H
#define TASKMAPCHANGE_H

#include "taskbase.h"

namespace game {

struct CTaskManager;

namespace editor {

struct CMapInterf;

enum class BrushSize : int
{
    Brush1x1,
    Brush3x3,
    Brush5x5
};

struct CTaskMapChangeData
{
    CTaskManager* taskManager;
    CMapInterf* mapInterface;
    BrushSize brushSize;
    int unknown;
    int unknown2;
    SmartPointer ptr;
    SmartPointer ptr2;
    SmartPointer ptr3;
};

assert_size(CTaskMapChangeData, 44);

/** Base class for map changing tools logic. */
struct CTaskMapChange : CTaskBase
{
    CTaskMapChangeData* mapChangeData;
};

} // namespace editor
} // namespace game

#endif // TASKMAPCHANGE_H
