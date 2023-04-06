/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#include "menunewskirmishsinglehooks.h"
#include "button.h"
#include "dialoginterf.h"
#include "log.h"
#include "menunewskirmishsingle.h"
#include "originalfunctions.h"

namespace hooks {

static void __fastcall showMenuRandomScenarioSingle(game::CMenuNewSkirmishSingle* thisptr,
                                                    int /*%edx*/)
{
    using namespace game;

    // Transfer to a new single player random scenario generation menu, from state 6 to 37
    CMenuPhase* menuPhase{thisptr->menuBaseData->menuPhase};
    CMenuPhaseApi::get().setTransition(menuPhase, 1);
}

game::CMenuNewSkirmishSingle* __fastcall menuNewSkirmishSingleCtorHooked(
    game::CMenuNewSkirmishSingle* thisptr,
    int /* %edx */,
    game::CMenuPhase* menuPhase)
{
    getOriginalFunctions().menuNewSkirmishSingleCtor(thisptr, menuPhase);

    using namespace game;

    static const char buttonName[] = "BTN_RANDOM_MAP";
    const auto& menuBase{CMenuBaseApi::get()};
    const auto& dialogApi{CDialogInterfApi::get()};

    CDialogInterf* dialog{menuBase.getDialogInterface(thisptr)};

    // Check if we have new button for random scenario generator menu and setup its callback
    if (dialogApi.findControl(dialog, buttonName)) {
        const auto& button{CButtonInterfApi::get()};

        SmartPointer functor;
        auto callback = (CMenuBaseApi::Api::ButtonCallback)showMenuRandomScenarioSingle;

        menuBase.createButtonFunctor(&functor, 0, thisptr, &callback);
        button.assignFunctor(dialog, buttonName, "DLG_CHOOSE_SKIRMISH", &functor, 0);
        SmartPointerApi::get().createOrFreeNoDtor(&functor, nullptr);
    }

    return thisptr;
}

} // namespace hooks
