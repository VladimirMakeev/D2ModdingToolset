/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#include "mainview2hooks.h"
#include "dialoginterf.h"
#include "gameimages.h"
#include "gameutils.h"
#include "isolayers.h"
#include "log.h"
#include "mainview2.h"
#include "mapgraphics.h"
#include "phasegame.h"
#include "scenarioinfo.h"
#include "togglebutton.h"
#include <fmt/format.h>

namespace hooks {

static bool gridVisible{false};

static void showGrid(int mapSize)
{
    using namespace game;

    const auto& imagesApi = GameImagesApi::get();

    GameImagesPtr imagesPtr;
    imagesApi.getGameImages(&imagesPtr);
    auto images = *imagesPtr.data;

    const auto& mapGraphics{MapGraphicsApi::get()};

    for (int x = 0; x < mapSize; ++x) {
        for (int y = 0; y < mapSize; ++y) {
            auto gridImage{imagesApi.getImage(images->isoCmon, "GRID", 0, true, images->log)};
            if (!gridImage) {
                continue;
            }

            const CMqPoint mapPosition{x, y};
            mapGraphics.showImageOnMap(&mapPosition, isoLayers().grid, gridImage, 0, 0);
        }
    }

    imagesApi.createOrFreeGameImages(&imagesPtr, nullptr);
}

static void hideGrid()
{
    using namespace game;

    MapGraphicsApi::get().hideLayerImages(isoLayers().grid);
}

static void __fastcall mainView2OnToggleGrid(game::CMainView2* thisptr,
                                             int /*%edx*/,
                                             bool toggleOn,
                                             game::CToggleButton*)
{
    gridVisible = toggleOn;

    if (gridVisible) {
        auto objectMap{game::CPhaseApi::get().getObjectMap(&thisptr->phaseGame->phase)};
        auto scenarioInfo{getScenarioInfo(objectMap)};

        showGrid(scenarioInfo->mapSize);
        return;
    }

    hideGrid();
}

void __fastcall mainView2ShowIsoDialogHooked(game::CMainView2* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto& mainViewApi{CMainView2Api::get()};

    mainViewApi.showDialog(thisptr, nullptr);

    static const char buttonName[]{"TOG_GRID"};

    const auto& dialogApi{CDialogInterfApi::get()};
    auto dialog{thisptr->dialogInterf};

    if (!dialogApi.findControl(dialog, buttonName)) {
        // Grid button was not added to Interf.dlg, skip
        return;
    }

    auto toggleButton{dialogApi.findToggleButton(dialog, buttonName)};
    if (!toggleButton) {
        // Control was found, but it is not CToggleButton
        logError("mssProxyError.log", fmt::format("{:s} in {:s} must be a toggle button",
                                                  buttonName, dialog->data->dialogName));
        return;
    }

    using ButtonCallback = CMainView2Api::Api::ToggleButtonCallback;

    ButtonCallback callback{};
    callback.callback = (ButtonCallback::Callback)&mainView2OnToggleGrid;

    SmartPointer functor;
    mainViewApi.createToggleButtonFunctor(&functor, 0, thisptr, &callback);

    const auto& buttonApi{CToggleButtonApi::get()};
    buttonApi.assignFunctor(dialog, buttonName, dialog->data->dialogName, &functor, 0);
    SmartPointerApi::get().createOrFreeNoDtor(&functor, nullptr);

    buttonApi.setChecked(toggleButton, gridVisible);
}

} // namespace hooks
