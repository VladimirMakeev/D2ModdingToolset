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

#ifndef UNITPICKERINTERF_H
#define UNITPICKERINTERF_H

#include "functordispatch3.h"
#include "midgardid.h"
#include "popupinterf.h"

namespace game {

struct CRadioButtonInterf;
struct LSubRaceCategory;

namespace editor {

/** RAD_RACE toggle button indices in DLG_ENROLL_UNITS from ScenEdit.dlg. */
enum class UnitPickerSubrace : int
{
    Human,
    Dwarf,
    Undead,
    Heretic,
    Neutral,
    NeutralHuman,
    NeutralElf,
    NeutralGreenSkin,
    NeutralDragon,
    NeutralMarsh,
    NeutralWater,
    NeutralBarbarian,
    NeutralWolf,
    Elf,
};

/** RAD_SORTED toggle button indices in DLG_ENROLL_UNITS from ScenEdit.dlg. */
enum class UnitPickerSortMethod : int
{
    Experience,
    Name,
};

struct CUnitPickerInterfData
{
    Vector<CMidgardID> units;         /**< Units which can be potentially picked. */
    Vector<CMidgardID> filteredUnits; /**< Subset of units to pick from, filtered by subrace. */
    ITask* task;
    SmartPtr<CBFunctorDispatch3<CMidgardID, int, bool>> onUnitPicked;
    UnitPickerSubrace selectedSubRace;
    bool enabledButtons[14];
    char padding[2];
};

static_assert(sizeof(CUnitPickerInterfData) == 64,
              "Size of CUnitPickerInterfData structure must be exactly 64 bytes");

/** Represents DLG_ENROLL_UNITS from ScenEdit.dlg. */
struct CUnitPickerInterf : public CPopupInterf
{
    CUnitPickerInterfData* data;
};

static_assert(sizeof(CUnitPickerInterf) == 28,
              "Size of CUnitPickerInterf structure must be exactly 28 bytes");

namespace CUnitPickerInterfApi {

struct Api
{
    using DataConstructor = CUnitPickerInterfData*(__thiscall*)(CUnitPickerInterfData* thisptr);
    DataConstructor dataConstructor;

    /** Sets RAD_RACE toggle buttons enabled states depending on enabledButtons array. */
    using UpdateRadioButton = void(__stdcall*)(const bool* enabledButtons,
                                               CRadioButtonInterf* radioButton,
                                               size_t buttonsTotal);
    UpdateRadioButton updateRadioButton;

    /** Updates enabledButtons and returns initial subrace selection. */
    using UpdateEnabledButtons = void(__thiscall*)(CUnitPickerInterf* thisptr,
                                                   UnitPickerSubrace* selectedSubRace);
    UpdateEnabledButtons updateEnabledButtons;

    using UpdateEditLevel = void(__thiscall*)(CUnitPickerInterf* thisptr, int selectedIndex);
    UpdateEditLevel updateEditLevel;

    /**
     * Filter unit picker units by specified subrace.
     * Updates filteredUnits array.
     */
    using FilterUnits = void(__thiscall*)(CUnitPickerInterf* thisptr,
                                          const LSubRaceCategory* subRace);
    FilterUnits filterUnits;

    /** Sorts filteredUnits array using specified method. */
    using SortUnits = void(__thiscall*)(CUnitPickerInterf* thisptr,
                                        UnitPickerSortMethod sortMethod);
    SortUnits sortUnits;

    /** Called when RAD_RACE toggle buttons selection changed. */
    using OnTogglePressed = void(__thiscall*)(CUnitPickerInterf* thisptr,
                                              UnitPickerSubrace selectedSubRace);
    OnTogglePressed onTogglePressed;

    UnitPickerSubrace* selectedButton;
    UnitPickerSortMethod* sortMethod;
};

Api& get();

} // namespace CUnitPickerInterfApi

} // namespace editor

} // namespace game

#endif // UNITPICKERINTERF_H
