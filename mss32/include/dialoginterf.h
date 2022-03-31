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

#ifndef DIALOGINTERFACE_H
#define DIALOGINTERFACE_H

#include "d2pair.h"
#include "d2set.h"
#include "interface.h"
#include <cstddef>

namespace game {

struct CButtonInterf;
struct CListBoxInterf;
struct CTextBoxInterf;
struct CPictureInterf;
struct CRadioButtonInterf;
struct CToggleButton;
struct CSpinButtonInterf;
struct CEditBoxInterf;
struct CScrollBarInterf;
struct CMidAutoDlgLog;
struct IMqImage2;

struct CDialogInterfData
{
    char dialogName[48];
    SmartPtr<CMidAutoDlgLog> log;
    Set<Pair<char[48] /* control name */, int /* child index */>> childControls;
    IMqImage2* background;
    CMqRect area;
};

static_assert(sizeof(CDialogInterfData) == 104,
              "Size of CDialogInterfData structure must be exactly 104 bytes");

/**
 * Holds ui elements created from AutoDialog script.
 * Represents DIALOG from Interf.dlg or ScenEdit.dlg files.
 */
struct CDialogInterf : public CInterface
{
    CDialogInterfData* data;
};

static_assert(sizeof(CDialogInterf) == 12,
              "Size of CDialogInterf structure must be exactly 12 bytes");

static_assert(offsetof(CDialogInterf, data) == 8,
              "CDialogInterf::dialogInterfData offset must be 8 bytes");

namespace CDialogInterfApi {

struct Api
{
    /** Searches for ui element with specific name, returns nullptr if not found. */
    using FindControl = CInterface*(__thiscall*)(CDialogInterf* thisptr, const char* controlName);
    FindControl findControl;

    template <typename T>
    using FindUiElement = T*(__stdcall*)(CDialogInterf* thisptr, const char* controlName);

    /** Searches for button ui element with specific name, returns nullptr if not found. */
    FindUiElement<CButtonInterf> findButton;

    /** Searches for list box ui element with specific name, returns nullptr if not found. */
    FindUiElement<CListBoxInterf> findListBox;

    /** Searches for text box ui element with specific name, returns nullptr if not found. */
    FindUiElement<CTextBoxInterf> findTextBox;

    /** Searches for picture ui element with specific name, returns nullptr if not found. */
    FindUiElement<CPictureInterf> findPicture;

    /** Searches for radio button ui element with specific name, returns nullptr if not found. */
    FindUiElement<CRadioButtonInterf> findRadioButton;

    /** Searches for toggle button ui element with specific name, returns nullptr if not found. */
    FindUiElement<CToggleButton> findToggleButton;

    /** Searches for spin button ui element with specific name, returns nullptr if not found. */
    FindUiElement<CSpinButtonInterf> findSpinButton;

    /** Searches for edit box ui element with specific name, returns nullptr if not found. */
    FindUiElement<CEditBoxInterf> findEditBox;

    /**
     * Searches for scroll bar ui element with specific name, returns nullptr if not found.
     * This function exists only in Scenario Editor.
     */
    FindUiElement<CScrollBarInterf> findScrollBar;

    /**
     * Shows hidden ui element with specified name.
     * @returns false if ui element was not found or could not be shown
     */
    using ShowControl = bool(__stdcall*)(CDialogInterf* dialog,
                                         const char* dialogName,
                                         const char* controlName);
    ShowControl showControl;

    /**
     * Hides ui element with specified name.
     * @returns false in case ui element could not be hidden.
     */
    using HideControl = bool(__thiscall*)(CDialogInterf* thisptr, const char* controlName);
    HideControl hideControl;
};

Api& get();

} // namespace CDialogInterfApi

} // namespace game

#endif // DIALOGINTERFACE_H
