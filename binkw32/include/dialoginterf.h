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

#ifndef DIALOGINTERFACE_H
#define DIALOGINTERFACE_H

#include "interface.h"
#include <cstddef>

namespace game {

struct CButtonInterf;
struct CListBoxInterf;
struct CTextBoxInterf;

/** Must be 104 bytes according to CDialogInterf constructor. */
struct CDialogInterfData
{
    char unknown[104];
};

/**
 * Assumption: holds ui elements prototypes created from AutoDialog script.
 */
struct CDialogInterf : public CInterface
{
    CDialogInterfData* dialogInterfData;
};

static_assert(sizeof(CDialogInterf) == 12,
              "Size of CDialogInterf structure must be exactly 12 bytes");

static_assert(offsetof(CDialogInterf, dialogInterfData) == 8,
              "CDialogInterf::dialogInterfData offset must be 8 bytes");

namespace CDialogInterfApi {

struct Api
{
    /** Searches for button ui element with specific name, returns nullptr if not found. */
    using FindButton = CButtonInterf*(__thiscall*)(CDialogInterf* thisptr, const char* controlName);
    FindButton findButton;

    /** Searches for list box ui element with specific name, returns nullptr if not found. */
    using FindListBox = CListBoxInterf*(__thiscall*)(CDialogInterf* thisptr,
                                                     const char* controlName);
    FindListBox findListBox;

    /** Searches for text box ui element with specific name, returns nullptr if not found. */
    using FindTextBox = CTextBoxInterf*(__thiscall*)(CDialogInterf* thisptr,
                                                     const char* controlName);
    FindTextBox findTextBox;
};

Api& get();

} // namespace CDialogInterfApi

} // namespace game

#endif // DIALOGINTERFACE_H
