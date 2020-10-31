/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#ifndef BUTTON_H
#define BUTTON_H

#include "interface.h"

namespace game {

struct CDialogInterf;
struct Functor;

/** Button ui element. */
struct CButtonInterf : public CInterface
{ };

namespace CButtonInterfApi {

struct Api
{
    /**
     * Assigns functor with callback to a button.
     * Searches for button element by its and dialog names.
     * Callback will be called each time the button is pressed, if not disabled.
     * @param[in] dialog used for button search.
     * @param[in] buttonName button to assign functor to.
     * @param[in] dialogName dialog (menu) where to search for button.
     * @param[in] functor pointer to existing functor initialized with callback.
     * @returns button found with callback set or nullptr if not found.
     */
    using AssignFunctor = CButtonInterf*(__stdcall*)(CDialogInterf* dialog,
                                                     const char* buttonName,
                                                     const char* dialogName,
                                                     Functor* functor,
                                                     int hotkey);
    AssignFunctor assignFunctor;
};

Api& get();

struct Vftable
{
    void* unknown[40];

    /** Enables or disables the button. */
    using SetEnabled = void(__thiscall*)(CButtonInterf* thisptr, bool enabled);
    SetEnabled setEnabled;

    void* unknown2[4];
};

static_assert(sizeof(Vftable) == 45 * sizeof(void*),
              "CButton vftable must have exactly 45 methods");

Vftable* vftable();

} // namespace CButtonInterfApi

} // namespace game

#endif // BUTTON_H
