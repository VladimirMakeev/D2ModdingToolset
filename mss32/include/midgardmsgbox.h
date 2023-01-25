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

#ifndef MIDGARDMSGBOX_H
#define MIDGARDMSGBOX_H

#include "popupdialoginterf.h"
#include "uievent.h"

namespace game {

struct CMidMsgBoxButtonHandler;
struct CMidMsgBoxActHandler;

struct CMidgardMsgBoxData
{
    CDialogInterf* dialog;
    UiEvent updateEvent;
    int unknown;
    void* userData;
    bool confirmButtonOnly;
    char padding[3];
    CMidMsgBoxButtonHandler* buttonHandler;
    CMidMsgBoxActHandler* actHandler;
};

assert_size(CMidgardMsgBoxData, 48);

/**
 * Message box ui element.
 * By default, represents DLG_MESSAGE_BOX from Interf.dlg.
 */
struct CMidgardMsgBox : public CPopupDialogInterf
{
    CMidgardMsgBoxData* data;
};

assert_size(CMidgardMsgBox, 20);
assert_offset(CMidgardMsgBox, data, 16);

namespace CMidgardMsgBoxApi {

struct Api
{
    /**
     * Creates message box.
     * @param[in] thisptr message box to initialize.
     * @param[in] message text to show.
     * @param showCancelButton if set to 1, creates message box with cancel button.
     * @param[in] buttonHandler handler logic to execute upon message box closing.
     * Handler object destroyed and its memory freed in message box destructor.
     * @param[in] actHandler action handler logic. Used in Scenario Editor for map import/export.
     * @param dialogName name of the custom dialog ui element to show instead of default,
     * or nullptr. Default dialog is 'DLG_MESSAGE_BOX' from Interf.dlg.
     */
    using Constructor = CMidgardMsgBox*(__thiscall*)(CMidgardMsgBox* thisptr,
                                                     const char* message,
                                                     int showCancelButton,
                                                     CMidMsgBoxButtonHandler* buttonHandler,
                                                     CMidMsgBoxActHandler* actHandler,
                                                     const char* dialogName);
    Constructor constructor;
};

Api& get();

} // namespace CMidgardMsgBoxApi

} // namespace game

#endif // MIDGARDMSGBOX_H
