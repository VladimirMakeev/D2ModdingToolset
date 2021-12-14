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

#include "registeraccountinterf.h"
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

struct CRegisterAccountInterf : public game::CPopupDialogInterf
{ };

static void closeRegisterAccountInterf(CRegisterAccountInterf* interf)
{
    hideInterface(interf);
    interf->vftable->destructor(interf, 1);
}

static void __fastcall tryRegisterAccount(CRegisterAccountInterf* thisptr, int /*%edx*/)
{
    logDebug("lobby.log", "User tries to create account");

    using namespace game;

    auto& dialogApi = CDialogInterfApi::get();
    auto dialog = *thisptr->dialog;

    auto accNameEdit = dialogApi.findEditBox(dialog, "EDIT_ACCOUNT_NAME");
    if (!accNameEdit) {
        closeRegisterAccountInterf(thisptr);
        return;
    }

    auto nicknameEdit = dialogApi.findEditBox(dialog, "EDIT_NICKNAME");
    if (!nicknameEdit) {
        closeRegisterAccountInterf(thisptr);
        return;
    }

    auto pwdEdit = dialogApi.findEditBox(dialog, "EDIT_PASSWORD");
    if (!pwdEdit) {
        closeRegisterAccountInterf(thisptr);
        return;
    }

    auto pwdQuestionEdit = dialogApi.findEditBox(dialog, "EDIT_PWD_QUESTION");
    if (!pwdQuestionEdit) {
        closeRegisterAccountInterf(thisptr);
        return;
    }

    auto pwdAnswerEdit = dialogApi.findEditBox(dialog, "EDIT_PWD_ANSWER");
    if (!pwdAnswerEdit) {
        closeRegisterAccountInterf(thisptr);
        return;
    }

    // TODO: better check each input separately and show meaningful info to the player
    const char* accountName = accNameEdit->data->editBoxData.inputString.string;
    const char* nickname = nicknameEdit->data->editBoxData.inputString.string;
    const char* password = pwdEdit->data->editBoxData.inputString.string;
    const char* pwdQuestion = pwdQuestionEdit->data->editBoxData.inputString.string;
    const char* pwdAnswer = pwdAnswerEdit->data->editBoxData.inputString.string;

    if (!tryCreateAccount(accountName, nickname, password, pwdQuestion, pwdAnswer)) {
        showMessageBox("Wrong account data");
        return;
    }

    closeRegisterAccountInterf(thisptr);
}

static void __fastcall cancelRegisterAccount(CRegisterAccountInterf* thisptr, int /*%edx*/)
{
    logDebug("lobby.log", "User canceled account creation");
    closeRegisterAccountInterf(thisptr);
}

CRegisterAccountInterf* createRegisterAccountInterf()
{
    using namespace game;

    static const char dialogName[]{"DLG_REGISTER_ACCOUNT"};

    auto interf = (CRegisterAccountInterf*)Memory::get().allocate(sizeof(CRegisterAccountInterf));
    CPopupDialogInterfApi::get().constructor(interf, dialogName, nullptr);

    auto& dialogApi = CDialogInterfApi::get();
    auto dialog = *interf->dialog;

    Functor functor;
    auto callback = (CMenuBaseApi::Api::ButtonCallback)cancelRegisterAccount;
    CMenuBaseApi::get().createButtonFunctor(&functor, 0, (CMenuBase*)interf, &callback);

    CButtonInterfApi::get().assignFunctor(dialog, "BTN_CANCEL", dialogName, &functor, 0);

    const auto freeFunctor = FunctorApi::get().createOrFree;
    freeFunctor(&functor, nullptr);

    callback = (CMenuBaseApi::Api::ButtonCallback)tryRegisterAccount;
    CMenuBaseApi::get().createButtonFunctor(&functor, 0, (CMenuBase*)interf, &callback);
    CButtonInterfApi::get().assignFunctor(dialog, "BTN_OK", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    return interf;
}

void showRegisterAccountDialog()
{
    showInterface(createRegisterAccountInterf());
}

} // namespace hooks
