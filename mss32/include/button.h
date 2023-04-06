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

#ifndef BUTTON_H
#define BUTTON_H

#include "d2list.h"
#include "d2string.h"
#include "functordispatch0.h"
#include "interface.h"
#include "smartptr.h"
#include "uievent.h"

namespace game {

struct CDialogInterf;
struct CButtonInterfVftable;
struct IMqImage2;
struct Tooltip;

enum class ButtonState : int
{
    Normal,
    Hovered,
    Clicked,
    Disabled
};

struct CButtonInterfData
{
    int childIndex;
    bool enabled;
    char padding[3];
    SmartPtr<CBFunctorDispatch0> onClickedFunctor;
    List<char> hotkeys;
    UiEvent timerEvent;
    int autorepeatTimeoutMs;
    IMqImage2* normalStateImage;
    IMqImage2* hoveredStateImage;
    IMqImage2* clickedStateImage;
    IMqImage2* disabledStateImage;
    bool pressed;
    char padding2[3];
    UiEvent buttonKeyEvent;
    unsigned int buttonKeyEventId;
    int unknown5;
    bool hotkeyPressed;
    char padding3[3];
    String hotkeysString;
    SmartPtr<Tooltip> tooltip;
    int unknown11;
};

assert_size(CButtonInterfData, 144);

/** Button ui element. */
struct CButtonInterf : public CInterfaceT<CButtonInterfVftable>
{
    CButtonInterfData* buttonData;
};

assert_size(CButtonInterf, 12);

struct CButtonInterfVftable : public CInterfaceVftable
{
    using SetFunctor = void(__thiscall*)(CButtonInterf* thisptr, SmartPointer* functor);
    SetFunctor setOnClickedFunctor;

    using ChangeHotkey = void(__thiscall*)(CButtonInterf* thisptr, char key);

    ChangeHotkey addHotkey;
    ChangeHotkey removeHotkey;

    using GetHotkeyCount = std::uint32_t(__thiscall*)(const CButtonInterf* thisptr);
    GetHotkeyCount getHotkeyCount;

    /** Actual return type unknown. */
    using GetHotkey = void*(__thiscall*)(CButtonInterf* thisptr, std::uint32_t* index);
    GetHotkey getHotkey;

    /** Sets image for specified button state. */
    using SetImage = void(__thiscall*)(CButtonInterf* thisptr, IMqImage2* image, ButtonState state);
    SetImage setImage;

    /** Enables or disables the button. */
    using SetEnabled = void(__thiscall*)(CButtonInterf* thisptr, bool value);
    SetEnabled setEnabled;

    /** Returns true if button is enabled. */
    using IsEnabled = bool(__thiscall*)(const CButtonInterf* thisptr);
    IsEnabled isEnabled;

    using SetAutorepeatTimeout = void(__thiscall*)(CButtonInterf* thisptr, int timeoutMs);
    SetAutorepeatTimeout setAutorepeatTimeout;

    using GetAutorepeatTimeout = int(__thiscall*)(const CButtonInterf* thisptr);
    GetAutorepeatTimeout getAutorepeatTimeout;

    using OnAutorepeat = void(__thiscall*)(CButtonInterf* thisptr);
    OnAutorepeat onAutorepeat;
};

assert_vftable_size(CButtonInterfVftable, 45);
assert_offset(CButtonInterfVftable, setEnabled, 160);

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
                                                     SmartPointer* functor,
                                                     int hotkey);
    AssignFunctor assignFunctor;
};

Api& get();

} // namespace CButtonInterfApi

} // namespace game

#endif // BUTTON_H
