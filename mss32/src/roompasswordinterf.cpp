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

#include "roompasswordinterf.h"
#include "button.h"
#include "dialoginterf.h"
#include "editboxinterf.h"
#include "functor.h"
#include "log.h"
#include "mempool.h"
#include "menubase.h"
#include "menucustomlobby.h"
#include "popupdialoginterf.h"
#include "textids.h"
#include "utils.h"

namespace hooks {

struct CRoomPasswordInterf : public game::CPopupDialogInterf
{
    RoomPasswordHandler onSuccess;
    RoomPasswordHandler onCancel;
    CMenuCustomLobby* menuLobby;
};

static void closeRoomPasswordInterf(CRoomPasswordInterf* interf)
{
    hideInterface(interf);
    interf->vftable->destructor(interf, 1);
}

static void cancelRoomPasswordInput(CRoomPasswordInterf* interf)
{
    interf->onCancel(interf->menuLobby);
    closeRoomPasswordInterf(interf);
}

static void __fastcall onOkPressed(CRoomPasswordInterf* thisptr, int /*%edx*/)
{
    logDebug("lobby.log", "User tries to enter room password");

    using namespace game;

    auto& dialogApi = CDialogInterfApi::get();
    auto dialog = *thisptr->dialog;

    auto pwdEdit = dialogApi.findEditBox(dialog, "EDIT_PASSWORD");
    if (!pwdEdit) {
        logDebug("lobby.log", "Edit password ui element not found");
        cancelRoomPasswordInput(thisptr);
        return;
    }

    const auto& pwdString = pwdEdit->data->editBoxData.inputString;
    const char* password = pwdString.string ? pwdString.string : "";
    auto menuLobby = thisptr->menuLobby;

    if (customLobbyCheckRoomPassword(menuLobby, password)) {
        auto onSuccess = thisptr->onSuccess;
        // Close menu and _then_ run success logic
        closeRoomPasswordInterf(thisptr);
        onSuccess(menuLobby);
    } else {
        auto message{getInterfaceText(textIds().lobby.wrongRoomPassword.c_str())};
        if (message.empty()) {
            message = "Wrong room password";
        }

        showMessageBox(message);
    }
}

static void __fastcall onCancelPressed(CRoomPasswordInterf* thisptr, int /*%edx*/)
{
    logDebug("lobby.log", "User canceled room password input");
    cancelRoomPasswordInput(thisptr);
}

static CRoomPasswordInterf* createRoomPasswordInterf(CMenuCustomLobby* menuLobby,
                                                     RoomPasswordHandler onSuccess,
                                                     RoomPasswordHandler onCancel)
{
    using namespace game;

    static const char dialogName[]{"DLG_ROOM_PASSWORD"};

    auto interf = (CRoomPasswordInterf*)Memory::get().allocate(sizeof(CRoomPasswordInterf));
    CPopupDialogInterfApi::get().constructor(interf, dialogName, nullptr);
    interf->onSuccess = onSuccess;
    interf->onCancel = onCancel;
    interf->menuLobby = menuLobby;

    const auto createFunctor = CMenuBaseApi::get().createButtonFunctor;
    const auto assignFunctor = CButtonInterfApi::get().assignFunctor;
    const auto freeFunctor = FunctorApi::get().createOrFree;

    auto& dialogApi = CDialogInterfApi::get();
    auto dialog = *interf->dialog;

    Functor functor;
    auto callback = (CMenuBaseApi::Api::ButtonCallback)onCancelPressed;
    createFunctor(&functor, 0, (CMenuBase*)interf, &callback);
    assignFunctor(dialog, "BTN_CANCEL", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    callback = (CMenuBaseApi::Api::ButtonCallback)onOkPressed;
    createFunctor(&functor, 0, (CMenuBase*)interf, &callback);
    assignFunctor(dialog, "BTN_OK", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    return interf;
}

void showRoomPasswordDialog(CMenuCustomLobby* menuLobby,
                            RoomPasswordHandler onSuccess,
                            RoomPasswordHandler onCancel)
{
    showInterface(createRoomPasswordInterf(menuLobby, onSuccess, onCancel));
}

} // namespace hooks
