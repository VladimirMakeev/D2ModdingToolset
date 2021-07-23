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

#ifndef POPUPINTERF_H
#define POPUPINTERF_H

#include "popupdialoginterf.h"
#include "taskmanagerholder.h"

namespace game {

struct CPopupInterfData
{
    CDialogInterf* dialog;
    CTaskManager* taskManager;
    ITask* task;
};

static_assert(sizeof(CPopupInterfData) == 12,
              "Size of CPopupInterfData structure must be exactly 12 bytes");

struct CPopupInterf
    : public CPopupDialogInterf
    , public ITaskManagerHolder
{
    CPopupInterfData* popupData;
};

static_assert(sizeof(CPopupInterf) == 24,
              "Size of CPopupInterf structure must be exactly 24 bytes");

static_assert(offsetof(CPopupInterf, popupData) == 20,
              "CPopupInterf::popupData offset must be 20 bytes");

} // namespace game

#endif // POPUPINTERF_H
