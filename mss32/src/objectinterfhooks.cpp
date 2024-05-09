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

#include "objectinterfhooks.h"
#include "log.h"
#include "mempool.h"
#include "objectinterf.h"
#include "originalfunctions.h"
#include "sitecategoryhooks.h"
#include "taskobjaddsite.h"
#include <fmt/format.h>

namespace hooks {

game::editor::CTaskObj* __fastcall createTaskObjHooked(game::ITaskManagerHolder* thisptr,
                                                       int /* %edx */)
{
    using namespace game;
    using namespace editor;

    CTaskObj* task = getOriginalFunctions().createTaskObj(thisptr);
    if (task) {
        return task;
    }

    const auto ptr = reinterpret_cast<std::uintptr_t>(thisptr) - 8u;
    CObjectInterf* objInterf = reinterpret_cast<CObjectInterf*>(ptr);

    // TOG_RES_MARKET radio button in DLG_OBJECTS, ScenEdit.dlg
    constexpr int togResMarket = 13;
    if (objInterf->objInterfData->selectedMode != togResMarket) {
        logError("mssProxyError.log", fmt::format("Unknown selection mode {:d}",
                                                  objInterf->objInterfData->selectedMode));
        return nullptr;
    }

    if (!customSiteCategories().exists) {
        logError("mssProxyError.log", fmt::format("Toggle button was added to the UI"
                                                  " but new site category does not exist."));
        return nullptr;
    }

    auto addSite = (CTaskObjAddSite*)Memory::get().allocate(sizeof(CTaskObjAddSite));
    CTaskObjAddSiteApi::get().constructor(addSite, objInterf,
                                          customSiteCategories().resourceMarket);
    return addSite;
}

} // namespace hooks
