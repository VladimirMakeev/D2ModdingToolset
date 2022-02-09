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

#include "menuprotocolhooks.h"
#include "dialoginterf.h"
#include "listbox.h"
#include "lobbyclient.h"
#include "mempool.h"
#include "menuflashwait.h"
#include "menuphase.h"
#include "menuphasehooks.h"
#include "midgard.h"
#include "netcustomservice.h"
#include "networkpeer.h"
#include "originalfunctions.h"
#include "uievent.h"
#include "utils.h"

namespace hooks {

struct CMenuCustomProtocol;

class LobbyServerConnectionCallback : public NetworkPeerCallbacks
{
public:
    LobbyServerConnectionCallback(CMenuCustomProtocol* menu)
        : menuProtocol{menu}
    { }

    ~LobbyServerConnectionCallback() override = default;

    void onPacketReceived(DefaultMessageIDTypes type,
                          SLNet::RakPeerInterface* peer,
                          const SLNet::Packet* packet) override;

private:
    CMenuCustomProtocol* menuProtocol;
};

struct CMenuCustomProtocol : public game::CMenuProtocol
{
    CMenuCustomProtocol(game::CMenuPhase* menuPhase)
        : menuWait{nullptr}
        , callback{this}
    {
        using namespace game;

        CMenuProtocolApi::get().constructor(this, menuPhase);

        auto dialog = CMenuBaseApi::get().getDialogInterface(this);
        auto listBox = CDialogInterfApi::get().findListBox(dialog, "TLBOX_PROTOCOL");
        if (listBox) {
            // One more entry for our custom protocol
            CListBoxInterfApi::get().setElementsTotal(listBox,
                                                      listBox->listBoxData->elementsTotal + 1);
        }
    }

    game::UiEvent timeoutEvent{};
    game::CMenuFlashWait* menuWait;
    LobbyServerConnectionCallback callback;
};

static void stopWaitingConnection(CMenuCustomProtocol* menu)
{
    using namespace game;

    UiEventApi::get().destructor(&menu->timeoutEvent);

    auto menuWait{menu->menuWait};
    if (menuWait) {
        hideInterface(menuWait);
        menuWait->vftable->destructor(menuWait, 1);
        menu->menuWait = nullptr;
    }

    auto netService{getNetService()};
    if (!netService) {
        return;
    }

    netService->lobbyPeer.removeCallback(&menu->callback);
}

static void showConnectionError(CMenuCustomProtocol* menu, const char* errorMessage)
{
    stopWaitingConnection(menu);
    showMessageBox(errorMessage);
}

void LobbyServerConnectionCallback::onPacketReceived(DefaultMessageIDTypes type,
                                                     SLNet::RakPeerInterface* peer,
                                                     const SLNet::Packet* packet)
{
    switch (type) {
    case ID_CONNECTION_ATTEMPT_FAILED:
        showConnectionError(menuProtocol, "Connection attempt failed");
        return;
    case ID_NO_FREE_INCOMING_CONNECTIONS:
        showConnectionError(menuProtocol, "Lobby server is full");
        return;
    case ID_ALREADY_CONNECTED:
        showConnectionError(menuProtocol, "Already connected.\nThis should never happen");
        return;

    case ID_CONNECTION_REQUEST_ACCEPTED: {
        std::string hash;
        if (!computeHash(gameFolder() / "Globals", hash)) {
            showConnectionError(menuProtocol, "Could not compute hash");
            return;
        }

        if (!tryCheckFilesIntegrity(hash.c_str())) {
            showConnectionError(menuProtocol, "Could not request game integrity check");
            return;
        }

        return;
    }

    case ID_FILES_INTEGRITY_RESULT: {
        stopWaitingConnection(menuProtocol);

        SLNet::BitStream input{packet->data, packet->length, false};
        input.IgnoreBytes(sizeof(SLNet::MessageID));

        bool checkPassed{false};
        input.Read(checkPassed);

        if (checkPassed) {
            // Switch to custom lobby window
            menuPhaseSetTransitionHooked(menuProtocol->menuBaseData->menuPhase, 0, 2);
            return;
        }

        showConnectionError(menuProtocol, "Game integrity check failed");
        return;
    }

    default:
        return;
    }
}

static void __fastcall menuProtocolTimeoutHandler(CMenuCustomProtocol* menu, int /*%edx*/)
{
    showConnectionError(menu, "Failed to connect.\nLobby server not responding");
}

void __fastcall menuProtocolDisplayCallbackHooked(game::CMenuProtocol* thisptr,
                                                  int /*%edx*/,
                                                  game::String* string,
                                                  bool a3,
                                                  int selectedIndex)
{
    using namespace game;

    auto dialog = CMenuBaseApi::get().getDialogInterface(thisptr);
    auto listBox = CDialogInterfApi::get().findListBox(dialog, "TLBOX_PROTOCOL");
    int lastIndex = -1;

    if (listBox) {
        lastIndex = listBox->listBoxData->elementsTotal - 1;
    }

    if (selectedIndex == lastIndex) {
        StringApi::get().initFromString(string, "Lobby server");
        return;
    }

    getOriginalFunctions().menuProtocolDisplayCallback(thisptr, string, a3, selectedIndex);
}

void __fastcall menuProtocolContinueHandlerHooked(CMenuCustomProtocol* thisptr, int /*%edx*/)
{
    using namespace game;

    auto dialog = CMenuBaseApi::get().getDialogInterface(thisptr);
    auto listBox = CDialogInterfApi::get().findListBox(dialog, "TLBOX_PROTOCOL");
    if (!listBox) {
        return;
    }

    auto data = listBox->listBoxData;
    if (data->selectedElement != data->elementsTotal - 1) {
        getOriginalFunctions().menuProtocolContinueHandler(thisptr);
        return;
    }

    auto& midgardApi = CMidgardApi::get();
    auto midgard = midgardApi.instance();
    // Delete old net service
    midgardApi.setNetService(midgard, nullptr, true, false);

    IMqNetService* service{nullptr};
    if (!createCustomNetService(&service)) {
        return;
    }

    midgardApi.setNetService(midgard, service, true, false);

    auto menuWait = (CMenuFlashWait*)Memory::get().allocate(sizeof(CMenuFlashWait));
    CMenuFlashWaitApi::get().constructor(menuWait);

    thisptr->menuWait = menuWait;
    showInterface(menuWait);

    auto netService{static_cast<CNetCustomService*>(service)};
    netService->lobbyPeer.addCallback(&thisptr->callback);

    // Stop attempting to connect after 10 seconds
    createTimerEvent(&thisptr->timeoutEvent, thisptr, menuProtocolTimeoutHandler, 10000);
}

game::CMenuProtocol* __stdcall menuProtocolCreateMenuHooked(game::CMenuPhase* menuPhase)
{
    using namespace game;

    auto menu = (CMenuCustomProtocol*)Memory::get().allocate(sizeof(CMenuCustomProtocol));
    new (menu) CMenuCustomProtocol(menuPhase);

    return menu;
}

} // namespace hooks
