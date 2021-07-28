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

#include "unitpickerinterf.h"
#include "version.h"
#include <array>

namespace game::editor::CUnitPickerInterfApi {

Api& get()
{
    // clang-format off
    static Api api{
        (Api::DataConstructor)0x44183e,
        (Api::UpdateRadioButton)0x441a71,
        (Api::UpdateEnabledButtons)0x441c86,
        (Api::UpdateEditLevel)0x44240e,
        (Api::FilterUnits)0x442680,
        (Api::SortUnits)0x442705,
        (Api::OnTogglePressed)0x442520,
        (UnitPickerSubrace*)0x663650,
        (UnitPickerSortMethod*)0x64ea08,
    };
    // clang-format on

    return api;
}

} // namespace game::editor::CUnitPickerInterfApi
