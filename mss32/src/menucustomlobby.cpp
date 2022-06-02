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

#include "menucustomlobby.h"
#include "autodialog.h"
#include "button.h"
#include "dialoginterf.h"
#include "dynamiccast.h"
#include "globaldata.h"
#include "image2fill.h"
#include "image2outline.h"
#include "image2text.h"
#include "listbox.h"
#include "lobbyclient.h"
#include "log.h"
#include "loginaccountinterf.h"
#include "mempool.h"
#include "menuflashwait.h"
#include "menuphase.h"
#include "midgard.h"
#include "netcustomplayerclient.h"
#include "netcustomservice.h"
#include "netcustomsession.h"
#include "netmessages.h"
#include "netmsgcallbacks.h"
#include "netmsgmapentry.h"
#include "racelist.h"
#include "registeraccountinterf.h"
#include "roompasswordinterf.h"
#include "textboxinterf.h"
#include "uimanager.h"
#include "utils.h"
#include <chrono>
#include <fmt/chrono.h>
#include <thread>

namespace hooks {

static void menuDeleteWaitMenu(CMenuCustomLobby* menu)
{
    if (menu->waitMenu) {
        hideInterface(menu->waitMenu);
        menu->waitMenu->vftable->destructor(menu->waitMenu, 1);
        menu->waitMenu = nullptr;
    }
}

static void menuUpdateAccountText(CMenuCustomLobby* menu, const char* accountName = nullptr)
{
    using namespace game;

    auto& menuBase = CMenuBaseApi::get();
    auto& dialogApi = CDialogInterfApi::get();
    auto dialog = menuBase.getDialogInterface(menu);
    auto textBox = dialogApi.findTextBox(dialog, "TXT_NICKNAME");

    if (accountName) {
        auto text{fmt::format("\\fLarge;\\vC;\\hC;{:s}", accountName)};
        CTextBoxInterfApi::get().setString(textBox, text.c_str());
        return;
    }

    CTextBoxInterfApi::get().setString(textBox, "");
}

static void menuUpdateButtons(CMenuCustomLobby* menu)
{
    using namespace game;

    const auto loggedIn{menu->loggedIn};
    auto& menuBase = CMenuBaseApi::get();
    auto& dialogApi = CDialogInterfApi::get();
    auto dialog = menuBase.getDialogInterface(menu);

    // Disable login button if player has already logged in.
    auto loginButton = dialogApi.findButton(dialog, "BTN_LOGIN");
    loginButton->vftable->setEnabled(loginButton, !loggedIn);

    // Disable logout and create, load, join buttons if player is not logged in.
    auto logoutButton = dialogApi.findButton(dialog, "BTN_LOGOUT");
    logoutButton->vftable->setEnabled(logoutButton, loggedIn);

    auto createButton = dialogApi.findButton(dialog, "BTN_CREATE");
    createButton->vftable->setEnabled(createButton, loggedIn);

    auto loadButton = dialogApi.findButton(dialog, "BTN_LOAD");
    loadButton->vftable->setEnabled(loadButton, loggedIn);

    auto joinButton = dialogApi.findButton(dialog, "BTN_JOIN");
    joinButton->vftable->setEnabled(joinButton, loggedIn);
}

void __fastcall menuDestructor(CMenuCustomLobby* thisptr, int /*%edx*/, char flags)
{
    using namespace game;

    menuDeleteWaitMenu(thisptr);

    if (thisptr->netMsgEntryData) {
        logDebug("transitions.log", "Delete custom lobby menu netMsgEntryData");
        NetMsgApi::get().freeEntryData(thisptr->netMsgEntryData);
    }

    logDebug("transitions.log", "Delete rooms list event");
    UiEventApi::get().destructor(&thisptr->roomsListEvent);

    removeLobbyCallbacks(thisptr->uiCallbacks.get());
    removeRoomsCallback(thisptr->roomsCallbacks.get());

    thisptr->uiCallbacks.reset(nullptr);
    thisptr->roomsCallbacks.reset(nullptr);

    thisptr->rooms.~vector();

    if (flags & 1) {
        logDebug("transitions.log", "Free custom menu memory");
        Memory::get().freeNonZero(thisptr);
    }
}

static void __fastcall menuRegisterAccountBtnHandler(CMenuCustomLobby*, int /*%edx*/)
{
    showRegisterAccountDialog();
}

static void __fastcall menuLoginAccountBtnHandler(CMenuCustomLobby*, int /*%edx*/)
{
    showLoginAccountDialog();
}

static void __fastcall menuLogoutAccountBtnHandler(CMenuCustomLobby*, int /*%edx*/)
{
    logDebug("lobby.log", "User logging out");
    logoutAccount();
}

static void __fastcall menuRoomsListSearchHandler(CMenuCustomLobby*, int /*%edx*/)
{
    logDebug("lobby.log", "Request fresh rooms list");
    trySearchRooms();
}

static void __fastcall menuCreateRoomBtnHandler(CMenuCustomLobby* thisptr, int /*%edx*/)
{
    auto menuPhase = thisptr->menuBaseData->menuPhase;
    // Pretend we are in CMenuMulti, transition to CMenuMultyHost
    menuPhase->data->transitionNumber = 4;

    logDebug("transitions.log",
             "Create room, pretend we are in CMenuMulti, transition to CMenuNewSkirmishMulti");
    game::CMenuPhaseApi::get().setTransition(menuPhase, 0);
}

static void __fastcall menuLoadBtnHandler(CMenuCustomLobby* thisptr, int /*%edx*/)
{
    // Pretend we are in CMenuMulti, transition to CMenuLoadSkirmishMulti
    auto menuPhase = thisptr->menuBaseData->menuPhase;
    menuPhase->data->transitionNumber = 4;
    menuPhase->data->loadScenario = true;

    logDebug("transitions.log",
             "Create room, pretend we are in CMenuMulti, transition to CMenuLoadSkirmishMulti");
    game::CMenuPhaseApi::get().setTransition(menuPhase, 2);
}

static int menuGetCurrentRoomIndex(CMenuCustomLobby* menuLobby)
{
    using namespace game;

    auto dialog = CMenuBaseApi::get().getDialogInterface(menuLobby);
    auto listBox = CDialogInterfApi::get().findListBox(dialog, "LBOX_ROOMS");

    return CListBoxInterfApi::get().selectedIndex(listBox);
}

static void menuTryJoinRoom(CMenuCustomLobby* menuLobby, const char* roomName)
{
    using namespace game;

    if (!tryJoinRoom(roomName)) {
        logDebug("transitions.log", "Failed to request room join");
    }

    // Rooms callback will notify us when its time to send game messages to server,
    // requesting version and info.
    auto& menuBase = CMenuBaseApi::get();
    auto& dialogApi = CDialogInterfApi::get();
    auto dialog = menuBase.getDialogInterface(menuLobby);

    // For now, disable join button
    auto buttonJoin = dialogApi.findButton(dialog, "BTN_JOIN");
    if (buttonJoin) {
        buttonJoin->vftable->setEnabled(buttonJoin, false);
    }

    menuDeleteWaitMenu(menuLobby);

    auto flashWait = (CMenuFlashWait*)Memory::get().allocate(sizeof(CMenuFlashWait));
    CMenuFlashWaitApi::get().constructor(flashWait);

    showInterface(flashWait);
    menuLobby->waitMenu = flashWait;
}

static void menuOnRoomPasswordCorrect(CMenuCustomLobby* menuLobby)
{
    logDebug("transitions.log", "Room password correct, trying to join a room");

    // Get current room name, try to join
    using namespace game;

    auto index = menuGetCurrentRoomIndex(menuLobby);
    if (index < 0 || index >= (int)menuLobby->rooms.size()) {
        return;
    }

    const auto& room = menuLobby->rooms[index];
    menuTryJoinRoom(menuLobby, room.name.c_str());
}

static void menuOnRoomPasswordCancel(CMenuCustomLobby* menuLobby)
{
    // Create rooms list event once again
    createTimerEvent(&menuLobby->roomsListEvent, menuLobby, menuRoomsListSearchHandler, 5000);
}

static void __fastcall menuJoinRoomBtnHandler(CMenuCustomLobby* thisptr, int /*%edx*/)
{
    using namespace game;

    logDebug("transitions.log", "Join room button pressed");

    auto index = menuGetCurrentRoomIndex(thisptr);
    if (index < 0 || index >= (int)thisptr->rooms.size()) {
        return;
    }

    const auto& room = thisptr->rooms[index];

    // Do fast check if room can be joined
    if (room.usedSlots >= room.totalSlots) {
        // Could not join game. Host did not report any available race.
        showMessageBox(getInterfaceText("X005TA0886"));
        return;
    }

    // Check if room protected with password, ask user to input
    if (!room.password.empty()) {
        // Remove rooms list callback so we don't mess with the list
        UiEventApi::get().destructor(&thisptr->roomsListEvent);
        showRoomPasswordDialog(thisptr, menuOnRoomPasswordCorrect, menuOnRoomPasswordCancel);
        return;
    }

    menuTryJoinRoom(thisptr, room.name.c_str());
}

static void __fastcall menuListBoxDisplayHandler(CMenuCustomLobby* thisptr,
                                                 int /*%edx*/,
                                                 game::ImagePointList* contents,
                                                 const game::CMqRect* lineArea,
                                                 int index,
                                                 bool selected)
{
    if (thisptr->rooms.empty()) {
        return;
    }

    using namespace game;

    auto& createFreePtr = SmartPointerApi::get().createOrFree;
    auto& imageApi = CImage2TextApi::get();

    const auto width = lineArea->p2.x - lineArea->p1.x;
    const auto height = lineArea->p2.y - lineArea->p1.y;
    const auto& room = thisptr->rooms[(size_t)index % thisptr->rooms.size()];

    // Width of table column border image in pixels
    const int columnBorderWidth{9};
    // 'Host' column with in pixels
    const int hostTextWidth{122};

    int offset = 0;

    // Host name
    {
        auto hostText = (CImage2Text*)Memory::get().allocate(sizeof(CImage2Text));
        imageApi.constructor(hostText, hostTextWidth, height);
        imageApi.setText(hostText, fmt::format("\\vC;{:s}", room.hostName).c_str());

        ImagePtrPointPair pair{};
        createFreePtr((SmartPointer*)&pair.first, hostText);
        pair.second.x = offset + 5;
        pair.second.y = 0;

        ImagePointListApi::get().add(contents, &pair);
        createFreePtr((SmartPointer*)&pair.first, nullptr);

        offset += hostTextWidth + columnBorderWidth;
    }

    // 'Description' column width in pixels
    const int descriptionWidth{452};

    // Room description
    {
        auto description = (CImage2Text*)Memory::get().allocate(sizeof(CImage2Text));
        imageApi.constructor(description, descriptionWidth, height);
        imageApi.setText(description, fmt::format("\\vC;{:s}", room.name).c_str());

        ImagePtrPointPair pair{};
        createFreePtr((SmartPointer*)&pair.first, description);
        pair.second.x = offset + 5;
        pair.second.y = 0;

        ImagePointListApi::get().add(contents, &pair);
        createFreePtr((SmartPointer*)&pair.first, nullptr);

        offset += descriptionWidth + columnBorderWidth;
    }

    // '#' column width
    const int playerCountWidth{44};

    // Number of players in room
    {
        auto playerCount = (CImage2Text*)Memory::get().allocate(sizeof(CImage2Text));
        imageApi.constructor(playerCount, playerCountWidth, height);
        imageApi
            .setText(playerCount,
                     fmt::format("\\vC;\\hC;{:d}/{:d}", room.usedSlots, room.totalSlots).c_str());

        ImagePtrPointPair pair{};
        createFreePtr((SmartPointer*)&pair.first, playerCount);
        pair.second.x = offset;
        pair.second.y = 0;

        ImagePointListApi::get().add(contents, &pair);
        createFreePtr((SmartPointer*)&pair.first, nullptr);

        offset += playerCountWidth + columnBorderWidth;
    }

    // Password protection status
    if (!room.password.empty()) {
        auto lockImage{AutoDialogApi::get().loadImage("ROOM_PROTECTED")};
        ImagePtrPointPair pair{};
        createFreePtr((SmartPointer*)&pair.first, lockImage);
        // Adjust lock image position to match with lock image in column header
        pair.second.x = offset + 3;
        pair.second.y = 1;

        ImagePointListApi::get().add(contents, &pair);
        createFreePtr((SmartPointer*)&pair.first, nullptr);
    }

    // Outline for selected element
    if (selected) {
        auto outline = (CImage2Outline*)Memory::get().allocate(sizeof(CImage2Outline));

        CMqPoint size{};
        size.x = width;
        size.y = height;

        game::Color color{};
        // 0x0 - transparent, 0xff - opaque
        std::uint32_t opacity{0xff};
        CImage2OutlineApi::get().constructor(outline, &size, &color, opacity);

        ImagePtrPointPair pair{};
        createFreePtr((SmartPointer*)&pair.first, outline);
        pair.second.x = 0;
        pair.second.y = 0;

        ImagePointListApi::get().add(contents, &pair);
        createFreePtr((SmartPointer*)&pair.first, nullptr);
    }
}

static void copyString(char** dest, const char* src)
{
    auto& memory = game::Memory::get();

    if (*dest) {
        memory.freeNonZero(*dest);
        *dest = nullptr;
    }

    if (src && *src) {
        const auto length = std::strlen(src);
        *dest = (char*)memory.allocate(length + 1);
        std::strncpy(*dest, src, length);
        (*dest)[length] = 0;
    }
}

static bool __fastcall menuAnsInfoMsgHandler(CMenuCustomLobby* menu,
                                             int /*%edx*/,
                                             const game::CMenusAnsInfoMsg* message,
                                             std::uint32_t idFrom)
{
    logDebug("netCallbacks.log", fmt::format("CMenusAnsInfoMsg received from 0x{:x}", idFrom));

    if (message->raceIds.length == 0) {
        customLobbyProcessJoinError(menu, "No available races");
        return true;
    }

    using namespace game;

    auto menuPhase = menu->menuBaseData->menuPhase;
    auto globalData = *GlobalDataApi::get().getGlobalData();
    auto racesTable = globalData->raceCategories;
    auto& findRaceById = LRaceCategoryTableApi::get().findCategoryById;
    auto& listApi = RaceCategoryListApi::get();

    auto raceList = &menuPhase->data->races;
    listApi.freeNodes(raceList);

    for (auto node = message->raceIds.head->next; node != message->raceIds.head;
         node = node->next) {
        const int raceId = static_cast<const int>(node->data);

        LRaceCategory category{};
        findRaceById(racesTable, &category, &raceId);

        listApi.add(raceList, &category);
    }

    menuPhase->data->unknown8 = false;
    menuPhase->data->suggestedLevel = message->suggestedLevel;

    copyString(&menuPhase->data->scenarioName, message->scenarioName);
    copyString(&menuPhase->data->scenarioDescription, message->scenarioDescription);

    logDebug("netCallbacks.log", "Switch to CMenuLobbyJoin (I hope)");
    // Here we can set next menu transition, there is no need to hide wait message,
    // it will be hidden from the destructor
    // Pretend we are in transition 15, after CMenuSession, transition to CMenuLobbyJoin
    menuPhase->data->transitionNumber = 15;
    menuPhase->data->host = false;

    logDebug("transitions.log",
             "Joining room, pretend we are in phase 15, transition to CMenuLobbyJoin");
    game::CMenuPhaseApi::get().setTransition(menuPhase, 0);

    return true;
}

static bool __fastcall menuGameVersionMsgHandler(CMenuCustomLobby* menu,
                                                 int /*%edx*/,
                                                 const game::CGameVersionMsg* message,
                                                 std::uint32_t idFrom)
{
    // Check server version
    if (message->gameVersion != 40) {
        // "You are trying to join a game with a newer or an older version of the game."
        customLobbyProcessJoinError(menu, getInterfaceText("X006ta0008").c_str());
        return true;
    }

    using namespace game;

    CMenusReqInfoMsg requestInfo;
    requestInfo.vftable = NetMessagesApi::getMenusReqInfoVftable();

    auto& midgardApi = CMidgardApi::get();
    auto midgard = midgardApi.instance();

    midgardApi.sendNetMsgToServer(midgard, &requestInfo);

    auto msg{fmt::format("CGameVersionMsg received from 0x{:x}", idFrom)};
    logDebug("netCallbacks.log", msg);

    return true;
}

static game::RttiInfo<game::CInterfaceVftable> menuLobbyRttiInfo;

void menuCustomLobbyCtor(CMenuCustomLobby* menu, game::CMenuPhase* menuPhase)
{
    using namespace game;

    const char dialogName[] = "DLG_CUSTOM_LOBBY";

    auto& menuBase = CMenuBaseApi::get();

    logDebug("transitions.log", "Call CMenuBase c-tor for CMenuCustomLobby");
    menuBase.constructor(menu, menuPhase);

    auto vftable = &menuLobbyRttiInfo.vftable;

    static bool firstTime{true};
    if (firstTime) {
        firstTime = false;

        // Reuse object locator for our custom menu.
        // We only need it for dynamic_cast<CAnimInterf>() to work properly without exceptions
        // when the game exits main loop and checks for current CInterface.
        // See DestroyAnimInterface() in IDA.
        replaceRttiInfo(menuLobbyRttiInfo, menu->vftable);

        vftable->destructor = (game::CInterfaceVftable::Destructor)&menuDestructor;
    }

    menu->vftable = vftable;

    logDebug("transitions.log", "Call createMenu for CMenuCustomLobby");
    menuBase.createMenu(menu, dialogName);

    std::vector<RoomInfo>().swap(menu->rooms);
    menu->loggedIn = false;

    const auto freeFunctor = SmartPointerApi::get().createOrFreeNoDtor;

    // Setup button handlers
    {
        auto dialog = menuBase.getDialogInterface(menu);

        SmartPointer functor;
        menuBase.createButtonFunctor(&functor, 0, menu, &menuBase.buttonBackCallback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_BACK", dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);

        auto callback = (CMenuBaseApi::Api::ButtonCallback)menuRegisterAccountBtnHandler;

        menuBase.createButtonFunctor(&functor, 0, menu, &callback);
        button.assignFunctor(dialog, "BTN_REGISTER", dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);

        callback = (CMenuBaseApi::Api::ButtonCallback)menuLoginAccountBtnHandler;
        menuBase.createButtonFunctor(&functor, 0, menu, &callback);
        button.assignFunctor(dialog, "BTN_LOGIN", dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);

        callback = (CMenuBaseApi::Api::ButtonCallback)menuLogoutAccountBtnHandler;
        menuBase.createButtonFunctor(&functor, 0, menu, &callback);
        button.assignFunctor(dialog, "BTN_LOGOUT", dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);

        auto& dialogApi = CDialogInterfApi::get();
        auto logoutButton = dialogApi.findButton(dialog, "BTN_LOGOUT");
        if (logoutButton) {
            logoutButton->vftable->setEnabled(logoutButton, false);
        }

        callback = (CMenuBaseApi::Api::ButtonCallback)menuCreateRoomBtnHandler;
        menuBase.createButtonFunctor(&functor, 0, menu, &callback);
        button.assignFunctor(dialog, "BTN_CREATE", dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);

        callback = (CMenuBaseApi::Api::ButtonCallback)menuLoadBtnHandler;
        menuBase.createButtonFunctor(&functor, 0, menu, &callback);
        button.assignFunctor(dialog, "BTN_LOAD", dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);

        callback = (CMenuBaseApi::Api::ButtonCallback)menuJoinRoomBtnHandler;
        menuBase.createButtonFunctor(&functor, 0, menu, &callback);
        button.assignFunctor(dialog, "BTN_JOIN", dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);

        auto buttonCreate = dialogApi.findButton(dialog, "BTN_CREATE");
        if (buttonCreate) {
            buttonCreate->vftable->setEnabled(buttonCreate, false);
        }

        auto buttonLoad = dialogApi.findButton(dialog, "BTN_LOAD");
        if (buttonLoad) {
            buttonLoad->vftable->setEnabled(buttonLoad, false);
        }

        auto buttonJoin = dialogApi.findButton(dialog, "BTN_JOIN");
        if (buttonJoin) {
            buttonJoin->vftable->setEnabled(buttonJoin, false);
        }

        auto displayCallback = (CMenuBaseApi::Api::ListBoxDisplayCallback)menuListBoxDisplayHandler;
        menuBase.createListBoxDisplayFunctor(&functor, 0, menu, &displayCallback);

        auto listBox = dialogApi.findListBox(dialog, "LBOX_ROOMS");
        if (listBox) {
            CListBoxInterfApi::get().assignDisplaySurfaceFunctor(dialog, "LBOX_ROOMS", dialogName,
                                                                 &functor);

            CListBoxInterfApi::get().setElementsTotal(listBox, 0);
        }

        freeFunctor(&functor, nullptr);
    }

    // Setup lobby callbacks
    {
        menu->uiCallbacks = std::make_unique<UiUpdateCallbacks>(menu);
        addLobbyCallbacks(menu->uiCallbacks.get());
    }

    // Setup ingame net message callbacks
    {
        logDebug("netCallbacks.log", "Allocate entry data");

        auto& netApi = NetMsgApi::get();
        netApi.allocateEntryData(menu->menuBaseData->menuPhase, &menu->netMsgEntryData);

        auto& entryApi = CNetMsgMapEntryApi::get();

        logDebug("netCallbacks.log", "Allocate CMenusAnsInfoMsg entry");

        auto infoCallback = (CNetMsgMapEntryApi::Api::MenusAnsInfoCallback)menuAnsInfoMsgHandler;
        auto entry = entryApi.allocateMenusAnsInfoEntry(menu, infoCallback);

        logDebug("netCallbacks.log", "Add CMenusAnsInfoMsg entry");
        netApi.addEntry(menu->netMsgEntryData, entry);

        logDebug("netCallbacks.log", "Allocate CGameVersionMsg entry");

        auto versionCallback = (CNetMsgMapEntryApi::Api::GameVersionCallback)
            menuGameVersionMsgHandler;
        entry = entryApi.allocateGameVersionEntry(menu, versionCallback);

        logDebug("netCallbacks.log", "Add CGameVersionMsg entry");
        netApi.addEntry(menu->netMsgEntryData, entry);
    }
}

game::CMenuBase* __stdcall createCustomLobbyMenu(game::CMenuPhase* menuPhase)
{
    auto menu = (CMenuCustomLobby*)game::Memory::get().allocate(sizeof(CMenuCustomLobby));
    std::memset(menu, 0, sizeof(CMenuCustomLobby));

    menuCustomLobbyCtor(menu, menuPhase);
    return menu;
}

void customLobbyShowError(const char* message)
{
    showMessageBox(fmt::format("\\fLarge;\\hC;Error\\fSmall;\n\n{:s}", message));
}

void customLobbyProcessLogin(CMenuCustomLobby* menu, const char* accountName)
{
    using namespace game;

    // Remember account that successfully logged in
    setCurrentLobbyPlayer(accountName);
    menuUpdateAccountText(menu, accountName);

    menu->loggedIn = true;
    menuUpdateButtons(menu);

    // Connect ui-related rooms callbacks
    menu->roomsCallbacks = std::make_unique<RoomsListCallbacks>(menu);
    addRoomsCallback(menu->roomsCallbacks.get());

    // Request rooms list as soon as possible, no need to wait for event
    trySearchRooms();

    // Add timer event that will send rooms list requests every 5 seconds
    createTimerEvent(&menu->roomsListEvent, menu, menuRoomsListSearchHandler, 5000);
}

void customLobbyProcessLogout(CMenuCustomLobby* menu)
{
    using namespace game;

    // Forget about logged account
    setCurrentLobbyPlayer(nullptr);
    menuUpdateAccountText(menu);

    menu->loggedIn = false;
    menuUpdateButtons(menu);

    // Remove timer event
    game::UiEventApi::get().destructor(&menu->roomsListEvent);

    // Disconnect ui-related rooms callbacks
    removeRoomsCallback(menu->roomsCallbacks.get());
    menu->roomsCallbacks.reset(nullptr);

    auto& menuBase = CMenuBaseApi::get();
    auto& dialogApi = CDialogInterfApi::get();
    auto dialog = menuBase.getDialogInterface(menu);
    auto listBox = dialogApi.findListBox(dialog, "LBOX_ROOMS");
    auto& listBoxApi = CListBoxInterfApi::get();

    // Clean up any rooms information to be safe
    listBoxApi.setElementsTotal(listBox, 0);
    menu->rooms.clear();
}

void customLobbySetRoomsInfo(CMenuCustomLobby* menu, std::vector<RoomInfo>&& rooms)
{
    using namespace game;

    auto& menuBase = CMenuBaseApi::get();
    auto& dialogApi = CDialogInterfApi::get();
    auto dialog = menuBase.getDialogInterface(menu);
    auto listBox = dialogApi.findListBox(dialog, "LBOX_ROOMS");
    auto& listBoxApi = CListBoxInterfApi::get();

    if (!menu->loggedIn) {
        listBoxApi.setElementsTotal(listBox, 0);
        menu->rooms.clear();
        return;
    }

    listBoxApi.setElementsTotal(listBox, (int)rooms.size());
    menu->rooms = std::move(rooms);
}

void customLobbyProcessJoinError(CMenuCustomLobby* menu, const char* message)
{
    using namespace game;

    menuDeleteWaitMenu(menu);

    auto& menuBase = CMenuBaseApi::get();
    auto& dialogApi = CDialogInterfApi::get();
    auto dialog = menuBase.getDialogInterface(menu);

    // TODO: check player entered a room in lobby server,
    // if so, leave it and enable join button in rooms callback
    auto buttonJoin = dialogApi.findButton(dialog, "BTN_JOIN");
    if (buttonJoin) {
        buttonJoin->vftable->setEnabled(buttonJoin, true);
    }

    customLobbyShowError(message);
}

bool customLobbyCheckRoomPassword(CMenuCustomLobby* menu, const char* password)
{
    using namespace game;

    auto index = menuGetCurrentRoomIndex(menu);
    if (index < 0 || index >= (int)menu->rooms.size()) {
        return false;
    }

    const auto& room = menu->rooms[index];
    return room.password == password;
}

} // namespace hooks
