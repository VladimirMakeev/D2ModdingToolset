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

#include "mapinterfhooks.h"
#include "dialoginterf.h"
#include "mapinterf.h"
#include "mempool.h"
#include "radiobuttoninterf.h"
#include "taskchangecolor.h"
#include "terrainhooks.h"

namespace hooks {

static game::editor::CTaskMapChange* createChangeColorTask(game::editor::CMapInterf* mapInterf)
{
    using namespace game;
    using namespace editor;

    const auto& dialog = CDialogInterfApi::get();

    SelectedTerrain selectedTerrain{SelectedTerrain::Heretic};

    const auto selectedAction{mapInterf->data->selectedAction};

    switch (selectedAction) {
    case MapChangeAction::ColorTerrainHeretic:
        selectedTerrain = SelectedTerrain::Heretic;
        break;
    case MapChangeAction::ColorTerrainDwarf:
        selectedTerrain = SelectedTerrain::Dwarf;
        break;
    case MapChangeAction::ColorTerrainHuman:
        selectedTerrain = SelectedTerrain::Human;
        break;
    case MapChangeAction::ColorTerrainUndead:
        selectedTerrain = SelectedTerrain::Undead;
        break;
    case MapChangeAction::ColorTerrainElf:
        selectedTerrain = SelectedTerrain::Elf;
        break;

    default:
        // Map custom actions to custom terrains
        if (selectedAction > MapChangeAction::ColorTerrainElf) {
            const int customAction = (int)selectedAction - (int)MapChangeAction::ColorTerrainElf;
            selectedTerrain = static_cast<SelectedTerrain>((int)SelectedTerrain::Elf
                                                           + customAction);
            break;
        }

        // Could not determine terrain
        return nullptr;
    }

    auto brushButton = dialog.findRadioButton(mapInterf->isoViewData->dialogInterf, "RAD_BRUSH");
    const auto brushSize = static_cast<BrushSize>(brushButton->data->selectedButton);

    auto taskManagerHolder = &mapInterf->taskManagerHolder;
    auto taskManager = taskManagerHolder->vftable->getTaskManager(taskManagerHolder);

    auto task = (editor::CTaskChangeColor*)Memory::get().allocate(sizeof(CTaskChangeColor));

    const auto& changeColor = CTaskChangeColorApi::get();
    changeColor.constructor(task, taskManager, mapInterf, selectedTerrain, brushSize);
    return task;
}

game::editor::CTaskMapChange* __fastcall createMapChangeTaskHooked(
    game::ITaskManagerHolder* thisptr,
    int /*%edx*/)
{
    using namespace game;
    using namespace editor;

    auto mapInterf = castTaskManagerToMapInterf(thisptr);

    const auto& mapInterfApi = CMapInterfApi::get();

    switch (mapInterf->data->selectedAction) {
    case MapChangeAction::Erase:
        return mapInterfApi.createDelMapObjectTask(mapInterf);

    case MapChangeAction::AddMountains:
        return mapInterfApi.createAddMountainsTask(mapInterf);

    case MapChangeAction::AddTrees:
        return mapInterfApi.createAddTreesTask(mapInterf);

    case MapChangeAction::PlaceNeutralTerrain:
    case MapChangeAction::PlaceWater:
        return mapInterfApi.createChangeHeightTask(mapInterf);

    case MapChangeAction::ColorTerrainHeretic:
    case MapChangeAction::ColorTerrainDwarf:
    case MapChangeAction::ColorTerrainHuman:
    case MapChangeAction::ColorTerrainUndead:
    case MapChangeAction::ColorTerrainElf:
    default:
        return createChangeColorTask(mapInterf);
    }
}

game::LTerrainCategory* __stdcall getTerrainBySelection(
    game::LTerrainCategory* terrain,
    game::editor::SelectedTerrain selectedTerrain)
{
    using namespace game;
    using namespace editor;

    const auto& terrains = TerrainCategories::get();

    switch (selectedTerrain) {
    case SelectedTerrain::Heretic:
        terrain->id = terrains.heretic->id;
        break;
    case SelectedTerrain::Dwarf:
        terrain->id = terrains.dwarf->id;
        break;
    case SelectedTerrain::Human:
        terrain->id = terrains.human->id;
        break;
    case SelectedTerrain::Undead:
        terrain->id = terrains.undead->id;
        break;
    case SelectedTerrain::Elf:
        terrain->id = terrains.elf->id;
        break;

    default:
        if (selectedTerrain > SelectedTerrain::Elf) {
            const size_t index = (int)selectedTerrain - (int)SelectedTerrain::Elf - 1;
            if (index < customTerrains().size()) {
                terrain->id = customTerrains()[index].category.id;
                break;
            }
        }

        terrain->id = (TerrainId)-1;
        break;
    }

    terrain->table = terrains.heretic->table;
    terrain->vftable = TerrainCategories::vftable();
    return terrain;
}

} // namespace hooks
