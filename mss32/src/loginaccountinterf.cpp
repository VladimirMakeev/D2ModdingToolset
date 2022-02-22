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

#include "loginaccountinterf.h"
#include "button.h"
#include "dialoginterf.h"
#include "editboxinterf.h"
#include "functor.h"
#include "lobbyclient.h"
#include "log.h"
#include "mempool.h"
#include "menubase.h"
#include "popupdialoginterf.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

struct CLoginAccountInterf : public game::CPopupDialogInterf
{ };

static void closeLoginAccountInterf(CLoginAccountInterf* interf)
{
    hideInterface(interf);
    interf->vftable->destructor(interf, 1);
}

static void __fastcall loginAccount(CLoginAccountInterf* thisptr, int /*%edx*/)
{
    logDebug("lobby.log", "User tries to log in");

    using namespace game;

    auto& dialogApi = CDialogInterfApi::get();
    auto dialog = *thisptr->dialog;

    auto accNameEdit = dialogApi.findEditBox(dialog, "EDIT_ACCOUNT_NAME");
    if (!accNameEdit) {
        logDebug("lobby.log", "Edit account ui element not found");
        closeLoginAccountInterf(thisptr);
        return;
    }

    auto pwdEdit = dialogApi.findEditBox(dialog, "EDIT_PASSWORD");
    if (!pwdEdit) {
        logDebug("lobby.log", "Edit password ui element not found");
        closeLoginAccountInterf(thisptr);
        return;
    }

    const char* accountName = accNameEdit->data->editBoxData.inputString.string;
    const char* password = pwdEdit->data->editBoxData.inputString.string;

    if (!tryLoginAccount(accountName, password)) {
        showMessageBox("Wrong user input");
        return;
    }

    closeLoginAccountInterf(thisptr);
}

static void __fastcall cancelLoginAccount(CLoginAccountInterf* thisptr, int /*%edx*/)
{
    logDebug("lobby.log", "User canceled logging in");
    closeLoginAccountInterf(thisptr);
}

static CLoginAccountInterf* createLoginAccountInterf()
{
    using namespace game;

    static const char dialogName[]{"DLG_LOGIN_ACCOUNT"};

    auto interf = (CLoginAccountInterf*)Memory::get().allocate(sizeof(CLoginAccountInterf));
    CPopupDialogInterfApi::get().constructor(interf, dialogName, nullptr);

    const auto createFunctor = CMenuBaseApi::get().createButtonFunctor;
    const auto assignFunctor = CButtonInterfApi::get().assignFunctor;
    const auto freeFunctor = FunctorApi::get().createOrFree;

    auto& dialogApi = CDialogInterfApi::get();
    auto dialog = *interf->dialog;

    Functor functor;
    auto callback = (CMenuBaseApi::Api::ButtonCallback)cancelLoginAccount;
    createFunctor(&functor, 0, (CMenuBase*)interf, &callback);
    assignFunctor(dialog, "BTN_CANCEL", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    callback = (CMenuBaseApi::Api::ButtonCallback)loginAccount;
    createFunctor(&functor, 0, (CMenuBase*)interf, &callback);
    assignFunctor(dialog, "BTN_OK", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    return interf;
}

void showLoginAccountDialog()
{
    showInterface(createLoginAccountInterf());
}

} // namespace hooks
