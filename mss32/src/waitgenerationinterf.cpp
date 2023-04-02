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

#include "waitgenerationinterf.h"
#include "button.h"
#include "mempool.h"
#include "menubase.h"

namespace hooks {

static void __fastcall waitCancelButtonHandler(WaitGenerationInterf* thisptr, int /*%edx*/)
{
    thisptr->onCanceled(thisptr->menu);
}

WaitGenerationInterf* createWaitGenerationInterf(CMenuRandomScenario* menu,
                                                 OnGenerationCanceled onCanceled)
{
    using namespace game;

    static const char waitDialogName[] = "DLG_WAIT_GENERATION";

    const auto& allocateMem{game::Memory::get().allocate};

    auto* interf = (WaitGenerationInterf*)allocateMem(sizeof(WaitGenerationInterf));
    CPopupDialogInterfApi::get().constructor(interf, waitDialogName, nullptr);

    interf->onCanceled = onCanceled;
    interf->menu = menu;

    CDialogInterf* dialog{*interf->dialog};

    SmartPointer functor;
    auto callback = (CMenuBaseApi::Api::ButtonCallback)waitCancelButtonHandler;
    CMenuBaseApi::get().createButtonFunctor(&functor, 0, (CMenuBase*)interf, &callback);
    CButtonInterfApi::get().assignFunctor(dialog, "BTN_CANCEL", waitDialogName, &functor, 0);
    SmartPointerApi::get().createOrFreeNoDtor(&functor, nullptr);

    return interf;
}

} // namespace hooks
