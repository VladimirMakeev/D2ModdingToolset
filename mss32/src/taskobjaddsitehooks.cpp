/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#include "taskobjaddsitehooks.h"
#include "taskobjaddsite.h"
#include "sitecategoryhooks.h"
#include "originalfunctions.h"
#include "visitors.h"
#include "scenedit.h"
#include "utils.h"
#include "editor.h"
#include "midsite.h"
#include "scenedithooks.h"
#include "game.h"
#include <fmt/format.h>

namespace hooks {

bool __fastcall taskObjAddSiteDoActionHooked(game::editor::CTaskObjAddSite* thisptr,
                                             int /*%edx*/,
                                             const game::CMqPoint* mapPosition)
{
    using namespace game;
    using namespace editor;

    const auto& marketCategory{customSiteCategories().resourceMarket};
    if (!customSiteCategories().exists || thisptr->siteData->siteCategory.id != marketCategory.id) {
        return getOriginalFunctions().taskObjAddSiteDoAction(thisptr, mapPosition);
    }

    // Add resource market site
    CScenEdit* scenEdit{CScenEditApi::get().instance()};
    IMidgardObjectMap* objectMap{scenEdit->data->unknown2->data->scenarioMap};

    const std::string name{getInterfaceText("X002TA0043")};
    const std::string description{getInterfaceText("X002TA0044")};

    if (!VisitorApi::get().createSite(&marketCategory, mapPosition, 0, "", name.c_str(),
                                      description.c_str(), objectMap, 1)) {
        showErrorMessageBox("Could not create resource market object");
        return false;
    }

    const CMidSite* site{editorFunctions.getSiteAtPosition(mapPosition, objectMap)};
    if (!site) {
        showErrorMessageBox(
            fmt::format("Could not find resource market after creation at ({:d}, {:d})",
                        mapPosition->x, mapPosition->y));
        return true;
    }

    if (site->siteCategory.id != marketCategory.id) {
        showErrorMessageBox(fmt::format("Wrong resource market site category {:d}, expected {:d}",
                                        (int)site->siteCategory.id, (int)marketCategory.id));
        return true;
    }
    
    const auto& names{getMarketNames()};
    const int index{gameFunctions().generateRandomNumberStd(names.size())};

    const auto& siteName{names[index].first};
    const auto& siteDescription{names[index].second};

    VisitorApi::get().changeSiteInfo(&site->id, siteName.c_str(), siteDescription.c_str(),
                                     objectMap, 1);

    editorFunctions.showOrHideSiteOnStrategicMap(site, objectMap, nullptr, 1);
    return false;
}

}
