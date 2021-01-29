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

#ifndef FULLSCREENINTERF_H
#define FULLSCREENINTERF_H

#include "interffullscreen.h"
#include "taskmanagerholder.h"

namespace game {

struct CTaskManager;

struct CFullScreenInterfData
{
    CTaskManager* taskManager;
    char unknown;
    char padding[3];
};

static_assert(sizeof(CFullScreenInterfData) == 8,
              "Size of CFullScreenInterfData structure must be exactly 8 bytes");

struct CFullScreenInterf : public CInterfFullScreen
{
    ITaskManagerHolder taskManagerHolder;
    CFullScreenInterfData* fullScreenInterfData;
};

} // namespace game

#endif // FULLSCREENINTERF_H
