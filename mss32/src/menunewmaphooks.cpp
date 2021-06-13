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

#include "menunewmaphooks.h"
#include "dialoginterf.h"
#include "editboxinterf.h"
#include "menunewmap.h"
#include "racehooks.h"
#include "spinbuttoninterf.h"
#include "togglebutton.h"
#include <fmt/format.h>

namespace hooks {

static void addRaceByToggle(game::CDialogInterf* dialog,
                            game::RaceCategoryList* races,
                            const game::LRaceCategory* race,
                            const char* buttonName)
{
    using namespace game;

    auto toggleButton = CDialogInterfApi::get().findToggleButton(dialog, buttonName);

    if (toggleButton && toggleButton->data->checked) {
        RaceCategoryListApi::get().add(races, race);
    }
}

void __stdcall getNewMapCreationSettingsHooked(game::CDialogInterf* dialog,
                                               game::String* scenarioName,
                                               int* mapSize,
                                               game::RaceCategoryList* races,
                                               const game::editor::CMenuNewMapData* data)
{
    using namespace game;
    using namespace editor;

    const auto& dialogApi = CDialogInterfApi::get();

    auto editBox = dialogApi.findEditBox(dialog, "EDIT_SCEN_NAME");
    auto nameString = editBox->data->editBoxData.inputString.string;

    StringApi::get().initFromString(scenarioName, nameString);

    auto spinButton = dialogApi.findSpinButton(dialog, "SPIN_MAP_SIZE");
    *mapSize = data->mapSizes.bgn[spinButton->data->selectedOption];

    const auto& raceCategories = RaceCategories::get();

    RaceCategoryListApi::get().add(races, raceCategories.neutral);

    addRaceByToggle(dialog, races, raceCategories.human, "TOG_HUMAN");
    addRaceByToggle(dialog, races, raceCategories.heretic, "TOG_HERETICS");
    addRaceByToggle(dialog, races, raceCategories.dwarf, "TOG_DWARF");
    addRaceByToggle(dialog, races, raceCategories.undead, "TOG_UNDEAD");
    addRaceByToggle(dialog, races, raceCategories.elf, "TOG_ELF");

    for (const auto& customRace : newRaces()) {
        const auto buttonName{fmt::format("TOG_{:s}", customRace.abbreviation)};
        addRaceByToggle(dialog, races, &customRace.category, buttonName.c_str());
    }
}

} // namespace hooks
