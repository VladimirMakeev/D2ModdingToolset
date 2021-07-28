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

#include "unitpickerinterfhooks.h"
#include "dialoginterf.h"
#include "listbox.h"
#include "originalfunctions.h"
#include "radiobuttoninterf.h"
#include "subracehooks.h"
#include "unitpickerinterf.h"
#include <algorithm>

namespace hooks {

game::editor::CUnitPickerInterfData* __fastcall unitPickerDataCtorHooked(
    game::editor::CUnitPickerInterfData* thisptr,
    int /*%edx*/)
{
    getOriginalFunctions().unitPickerDataCtor(thisptr);

    std::memset(thisptr->enabledButtons, 0, sizeof(thisptr->enabledButtons));
    return thisptr;
}

void __stdcall unitPickerUpdateRadioButtonHooked(const bool* enabledButtons,
                                                 game::CRadioButtonInterf* radioButton,
                                                 size_t)
{
    const auto& setButtonEnabled = game::CRadioButtonInterfApi::get().setButtonEnabled;

    for (int i = 0; i < 16; ++i) {
        setButtonEnabled(radioButton, i, enabledButtons[i]);
    }
}

static void updateEnabledButtons(game::editor::CUnitPickerInterf* unitPicker,
                                 game::editor::UnitPickerSubrace* selectedSubRace,
                                 const game::LSubRaceCategory* subrace,
                                 game::editor::UnitPickerSubrace subraceIndex)
{
    game::editor::CUnitPickerInterfApi::get().filterUnits(unitPicker, subrace);

    const auto& filteredUnits = unitPicker->data->filteredUnits;

    if (filteredUnits.end - filteredUnits.bgn) {
        unitPicker->data->enabledButtons[static_cast<int>(subraceIndex)] = true;
        *selectedSubRace = subraceIndex;
    }
}

void __fastcall unitPickerUpdateEnabledButtonsHooked(
    game::editor::CUnitPickerInterf* thisptr,
    int /*%edx*/,
    game::editor::UnitPickerSubrace* selectedSubRace)
{
    using namespace game;
    using namespace editor;

    *selectedSubRace = UnitPickerSubrace::Human;

    // Use two new subraces maximum because of CUnitPickerInterfData::padding reusing.
    const auto newSubracesCount = std::min(newSubRaces().size(),
                                           sizeof(CUnitPickerInterfData::original.padding));

    for (size_t i = 0; i < newSubracesCount; ++i) {
        const auto& subrace = newSubRaces()[i];
        constexpr int customIndex = (int)UnitPickerSubrace::Elf + 1;
        updateEnabledButtons(thisptr, selectedSubRace, &subrace.category,
                             (UnitPickerSubrace)(customIndex + i));
    }

    const auto& subraces = SubraceCategories::get();

    updateEnabledButtons(thisptr, selectedSubRace, subraces.elf, UnitPickerSubrace::Elf);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.neutralWolf,
                         UnitPickerSubrace::NeutralWolf);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.neutralBarbarian,
                         UnitPickerSubrace::NeutralBarbarian);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.neutralWater,
                         UnitPickerSubrace::NeutralWater);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.neutralMarsh,
                         UnitPickerSubrace::NeutralMarsh);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.neutralDragon,
                         UnitPickerSubrace::NeutralDragon);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.neutralGreenSkin,
                         UnitPickerSubrace::NeutralGreenSkin);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.neutralElf,
                         UnitPickerSubrace::NeutralElf);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.neutralHuman,
                         UnitPickerSubrace::NeutralHuman);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.neutral, UnitPickerSubrace::Neutral);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.heretic, UnitPickerSubrace::Heretic);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.undead, UnitPickerSubrace::Undead);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.dwarf, UnitPickerSubrace::Dwarf);
    updateEnabledButtons(thisptr, selectedSubRace, subraces.human, UnitPickerSubrace::Human);
}

void __fastcall unitPickerOnTogglePressedHooked(game::editor::CUnitPickerInterf* thisptr,
                                                int /*%edx*/,
                                                game::editor::UnitPickerSubrace selectedSubRace)
{
    using namespace game;
    using namespace editor;

    int buttonIndex = (int)selectedSubRace;

    while (thisptr->data->enabledButtons[buttonIndex] != true) {
        --buttonIndex;
        if (buttonIndex < 0) {
            buttonIndex = 15;
        }
    }

    auto& unitPicker = CUnitPickerInterfApi::get();

    const UnitPickerSubrace newSelection = (UnitPickerSubrace)buttonIndex;
    *unitPicker.selectedButton = newSelection;

    const auto& filterUnits = unitPicker.filterUnits;
    const auto& subraces = SubraceCategories::get();

    switch (newSelection) {
    case UnitPickerSubrace::Human:
        filterUnits(thisptr, subraces.human);
        break;
    case UnitPickerSubrace::Dwarf:
        filterUnits(thisptr, subraces.dwarf);
        break;
    case UnitPickerSubrace::Undead:
        filterUnits(thisptr, subraces.undead);
        break;
    case UnitPickerSubrace::Heretic:
        filterUnits(thisptr, subraces.heretic);
        break;
    case UnitPickerSubrace::Neutral:
        filterUnits(thisptr, subraces.neutral);
        break;
    case UnitPickerSubrace::NeutralHuman:
        filterUnits(thisptr, subraces.neutralHuman);
        break;
    case UnitPickerSubrace::NeutralElf:
        filterUnits(thisptr, subraces.neutralElf);
        break;
    case UnitPickerSubrace::NeutralGreenSkin:
        filterUnits(thisptr, subraces.neutralGreenSkin);
        break;
    case UnitPickerSubrace::NeutralDragon:
        filterUnits(thisptr, subraces.neutralDragon);
        break;
    case UnitPickerSubrace::NeutralMarsh:
        filterUnits(thisptr, subraces.neutralMarsh);
        break;
    case UnitPickerSubrace::NeutralWater:
        filterUnits(thisptr, subraces.neutralWater);
        break;
    case UnitPickerSubrace::NeutralBarbarian:
        filterUnits(thisptr, subraces.neutralBarbarian);
        break;
    case UnitPickerSubrace::NeutralWolf:
        filterUnits(thisptr, subraces.neutralWolf);
        break;
    case UnitPickerSubrace::Elf:
        filterUnits(thisptr, subraces.elf);
        break;
    default: {
        const int index = buttonIndex - (int)UnitPickerSubrace::Elf - 1;
        if (index >= 0 && index < (int)sizeof(CUnitPickerInterfData::original.padding)) {
            filterUnits(thisptr, &newSubRaces()[index].category);
        }
    }
    }

    const auto& filteredUnits = thisptr->data->filteredUnits;
    const int unitsTotal = filteredUnits.end - filteredUnits.bgn;

    const auto& dialogApi = CDialogInterfApi::get();
    auto listBox = dialogApi.findListBox(*thisptr->dialog, "LBOX_UNITS");

    const auto& listBoxApi = CListBoxInterfApi::get();
    listBoxApi.setElementsTotal(listBox, unitsTotal);

    thisptr->data->selectedSubRace = newSelection;

    unitPicker.sortUnits(thisptr, *unitPicker.sortMethod);
    unitPicker.updateEditLevel(thisptr, listBoxApi.selectedIndex(listBox));

    auto radioButton = dialogApi.findRadioButton(*thisptr->dialog, "RAD_RACE");
    unitPickerUpdateRadioButtonHooked(thisptr->data->enabledButtons, radioButton, 16);

    CRadioButtonInterfApi::get().setCheckedButton(radioButton, buttonIndex);
}

} // namespace hooks
