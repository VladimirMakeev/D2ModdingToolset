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

#ifndef UNITPICKERINTERFHOOKS_H
#define UNITPICKERINTERFHOOKS_H

namespace game {
struct CRadioButtonInterf;

namespace editor {
struct CUnitPickerInterf;
struct CUnitPickerInterfData;
enum class UnitPickerSubrace;

} // namespace editor
} // namespace game

namespace hooks {

game::editor::CUnitPickerInterfData* __fastcall unitPickerDataCtorHooked(
    game::editor::CUnitPickerInterfData* thisptr,
    int /*%edx*/);

void __stdcall unitPickerUpdateRadioButtonHooked(const bool* enabledButtons,
                                                 game::CRadioButtonInterf* radioButton,
                                                 size_t buttonsTotal);

void __fastcall unitPickerUpdateEnabledButtonsHooked(
    game::editor::CUnitPickerInterf* thisptr,
    int /*%edx*/,
    game::editor::UnitPickerSubrace* selectedSubRace);

void __fastcall unitPickerOnTogglePressedHooked(game::editor::CUnitPickerInterf* thisptr,
                                                int /*%edx*/,
                                                game::editor::UnitPickerSubrace selectedSubRace);

} // namespace hooks

#endif // UNITPICKERINTERFHOOKS_H
