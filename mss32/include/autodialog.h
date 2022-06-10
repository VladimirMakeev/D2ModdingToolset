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

#ifndef AUTODIALOG_H
#define AUTODIALOG_H

#include "d2assert.h"
#include "d2set.h"
#include "d2string.h"
#include "smartptr.h"
#include "stringarray.h"

namespace game {

struct IMqImage2;
struct CMidAutoDlgImages;
struct CMidAutoDlgLog;
struct CMidAutoDlgTextLoader;

/**
 * Stores AutoDialog script (.dlg) information.
 */
struct DialogScriptData
{
    bool initialized;
    char padding[3];
    int unknown;
    StringArray lines; /**< Script file text lines. */
    String scriptPath; /**< Full path to script (.dlg) file. */
};

struct CAutoDialogData
{
    SmartPtr<CMidAutoDlgImages> images;
    SmartPtr<CMidAutoDlgLog> log;
    SmartPtr<CMidAutoDlgTextLoader> textLoader;
    SmartPointer memPool;
    Set<char[52]> dialogSet;
};

assert_size(CAutoDialogData, 60);

/** Holds necessary data to create CInterface objects from .dlg files. */
struct CAutoDialog
{
    CAutoDialogData* data;
};

assert_size(CAutoDialog, 4);

namespace AutoDialogApi {

struct Api
{
    /**
     * Loads AutoDialog script file and populates AutoDialogData with its contents.
     * @param[in] thisptr pointer to AutoDialogData where to store the data.
     * @param[in] filePath full path to script file.
     * @param unknown
     * @returns thisptr.
     */
    using LoadScriptFile = DialogScriptData*(__thiscall*)(DialogScriptData* thisptr,
                                                          const char* filePath,
                                                          int /*unknown*/);
    LoadScriptFile loadScriptFile;

    using LoadImage = IMqImage2*(__stdcall*)(const char* imageName);
    LoadImage loadImage;
};

/** Returns AutoDialog functions according to determined version of the game. */
Api& get();

} // namespace AutoDialogApi

} // namespace game

#endif // AUTODIALOG_H
