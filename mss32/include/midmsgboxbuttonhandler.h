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

#ifndef MIDMSGBOXBUTTONHANDLER_H
#define MIDMSGBOXBUTTONHANDLER_H

namespace game {

struct CMidMsgBoxButtonHandlerVftable;
struct CMidgardMsgBox;

/** Base class for all CMidgardMsgBox button handlers. */
struct CMidMsgBoxButtonHandler
{
    CMidMsgBoxButtonHandlerVftable* vftable;
};

static_assert(sizeof(CMidMsgBoxButtonHandler) == 4,
              "Size of CMidMsgBoxButtonHandler structure must be exactly 4 bytes");

struct CMidMsgBoxButtonHandlerVftable
{
    using Destructor = void(__thiscall*)(CMidMsgBoxButtonHandler* thisptr);
    Destructor destructor;

    /**
     * CMidgardMsgBox result handler function.
     * Called on BTN_YES, BTN_OK and BTN_NO presses.
     */
    using Handler = void(__thiscall*)(CMidMsgBoxButtonHandler* thisptr,
                                      CMidgardMsgBox* msgBox,
                                      bool okPressed);
    Handler handler;
};

static_assert(sizeof(CMidMsgBoxButtonHandlerVftable) == 2 * sizeof(void*),
              "CMidMsgBoxButtonHandler vftable must have exactly 2 methods");

} // namespace game

#endif // MIDMSGBOXBUTTONHANDLER_H
