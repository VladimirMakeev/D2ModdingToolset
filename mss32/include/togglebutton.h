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

#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "interface.h"
#include "smartptr.h"

namespace game {

struct CToggleButtonVftable;
struct CDialogInterf;

struct CToggleButtonData
{
    int buttonChildIndex;
    bool checked;
    char padding[3];
    SmartPointer ptr;
    SmartPointer ptrArray[7];
};

assert_size(CToggleButtonData, 72);

/**
 * Toggle button ui element.
 * Represents TOGGLE from Interf.dlg or ScenEdit.dlg files.
 */
struct CToggleButton : public CInterfaceT<CToggleButtonVftable>
{
    CToggleButtonData* data;
};

assert_size(CToggleButton, 12);

struct CToggleButtonVftable : public CInterfaceVftable
{
    void* method34;

    using SetEnabled = void(__thiscall*)(CToggleButton* thisptr, bool value);
    SetEnabled setEnabled;

    void* method36;
    void* method37;
};

assert_vftable_size(CToggleButtonVftable, 38);
assert_offset(CToggleButtonVftable, setEnabled, 140);

namespace CToggleButtonApi {

struct Api
{
    using SetChecked = void(__thiscall*)(CToggleButton* thisptr, bool checked);
    SetChecked setChecked;

    using AssignFunctor = CToggleButton*(__stdcall*)(CDialogInterf* dialog,
                                                     const char* buttonName,
                                                     const char* dialogName,
                                                     SmartPointer* functor,
                                                     int hotkey);
    AssignFunctor assignFunctor;
};

Api& get();

} // namespace CToggleButtonApi

} // namespace game

#endif // TOGGLEBUTTON_H
