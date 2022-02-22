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

#include "menunewskirmishhooks.h"
#include "dialoginterf.h"
#include "editboxinterf.h"
#include "lobbyclient.h"
#include "log.h"
#include "netcustomplayerserver.h"
#include "netcustomservice.h"
#include "netcustomsession.h"
#include "originalfunctions.h"
#include "roomservercreation.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

static bool isRoomAndPlayerNamesValid(game::CMenuBase* menu)
{
    using namespace game;

    auto& menuApi = CMenuBaseApi::get();
    auto& dialogApi = CDialogInterfApi::get();

    auto dialog = menuApi.getDialogInterface(menu);
    auto editGame = dialogApi.findEditBox(dialog, "EDIT_GAME");
    if (!editGame || !std::strlen(editGame->data->editBoxData.inputString.string)) {
        return false;
    }

    auto editName = dialogApi.findEditBox(dialog, "EDIT_NAME");
    if (!editName || !std::strlen(editName->data->editBoxData.inputString.string)) {
        return false;
    }

    return true;
}

void __fastcall menuNewSkirmishLoadScenarioCallbackHooked(game::CMenuBase* thisptr, int /*%edx*/)
{
    auto service = getNetService();
    if (!service) {
        // Current net service is not custom lobby, use default game logic
        getOriginalFunctions().menuNewSkirmishLoadScenario(thisptr);
        return;
    }

    logDebug("lobby.log", "Custom lobby net service!");

    if (!isRoomAndPlayerNamesValid(thisptr)) {
        // Please enter valid game and player names
        showMessageBox(getTranslatedText("X005TA0867"));
        return;
    }

    startRoomAndServerCreation(thisptr, false);
}

bool __fastcall menuNewSkirmishMultiCreateServerHooked(game::CMenuNewSkirmishMulti* thisptr,
                                                       int /*%edx*/)
{
    logDebug("lobby.log", "CMenuNewSkirmishMulti::CreateServer");

    const auto result{getOriginalFunctions().menuNewSkirmishMultiCreateServer(thisptr)};
    if (!result) {
        // Game failed to initialize session, server or host client
        logDebug("lobby.log", "Failed to create server");
        return false;
    }

    auto service{getNetService()};
    if (!service) {
        // Current net service is not custom lobby, use default game logic
        return result;
    }

    auto session{service->session};
    if (!session) {
        logDebug("lobby.log", "Session is null");
        return false;
    }

    auto playerServer{session->server};
    if (!playerServer) {
        logDebug("lobby.log", "Player server is null");
        return false;
    }

    logDebug("lobby.log", "Notify player server about host client connection");
    // Notify server about host player client connection.
    // The other clients that connect later will be handled in a usual way using net peer callbacks
    return playerServer->notifyHostClientConnected();
}

} // namespace hooks
