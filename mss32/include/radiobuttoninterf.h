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

#ifndef RADIOBUTTONINTERF_H
#define RADIOBUTTONINTERF_H

#include "d2pair.h"
#include "functordispatch1.h"
#include "interface.h"

namespace game {

struct CToggleButton;
struct CDialogInterf;
struct Functor;

struct CRadioButtonInterfData
{
    Vector<Pair<CToggleButton*, bool /* enabled */>> buttons;
    bool enabled;
    char padding[3];
    int selectedButton;
    SmartPtr<CBFunctorDispatch1<int>> onButtonPressed;
};

assert_size(CRadioButtonInterfData, 32);

/**
 * Radio button ui element.
 * Represents RADIO from Interf.dlg or ScenEdit.dlg files.
 */
struct CRadioButtonInterf : public CInterface
{
    CRadioButtonInterfData* data;
};

assert_size(CRadioButtonInterf, 12);

namespace CRadioButtonInterfApi {

struct Api
{
    /** Enables or disables radio button. */
    using SetEnabled = void(__thiscall*)(CRadioButtonInterf* thisptr, bool enabled);
    SetEnabled setEnabled;

    /** Enables or disables toggle button with specified index. */
    using SetButtonEnabled = void(__thiscall*)(CRadioButtonInterf* thisptr,
                                               int buttonIndex,
                                               bool enabled);
    SetButtonEnabled setButtonEnabled;

    /** Sets toggle button with specified index checked. */
    using SetCheckedButton = void(__thiscall*)(CRadioButtonInterf* thisptr, int buttonIndex);
    SetCheckedButton setCheckedButton;

    /** Assigns callback that is called when one of the radio buttons is pressed. */
    using SetOnButtonPressed = CRadioButtonInterf*(__stdcall*)(CDialogInterf* dialog,
                                                               const char* buttonName,
                                                               const char* dialogName,
                                                               Functor* functor);
    SetOnButtonPressed setOnButtonPressed;
};

Api& get();

} // namespace CRadioButtonInterfApi

} // namespace game

#endif // RADIOBUTTONINTERF_H
