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

#ifndef MENUCUSTOMLOBBY_H
#define MENUCUSTOMLOBBY_H

#include "lobbycallbacks.h"
#include "menubase.h"
#include "uievent.h"
#include <memory>
#include <string>
#include <vector>

namespace game {
struct NetMsgEntryData;
struct CMenuFlashWait;
} // namespace game

namespace hooks {

struct RoomInfo
{
    std::string name;
    std::string hostName;
    int publicSlots;
    int remainingSlots;
};

struct CMenuCustomLobby : public game::CMenuBase
{
    game::UiEvent roomsListEvent;
    std::vector<RoomInfo> rooms; // cached data
    std::unique_ptr<UiUpdateCallbacks> uiCallbacks;
    std::unique_ptr<RoomsListCallbacks> roomsCallbacks;
    game::NetMsgEntryData** netMsgEntryData;
    game::CMenuFlashWait* waitMenu;
    bool loggedIn;
};

game::CMenuBase* __stdcall createCustomLobbyMenu(game::CMenuPhase* menuPhase);

void customLobbyShowError(const char* message);

void customLobbyProcessLogin(CMenuCustomLobby* menu, const char* accountName);

void customLobbyProcessLogout(CMenuCustomLobby* menu);

void customLobbySetRoomsInfo(CMenuCustomLobby* menu, std::vector<RoomInfo>&& rooms);

void customLobbyProcessJoinError(CMenuCustomLobby* menu, const char* message);

} // namespace hooks

#endif // MENUCUSTOMLOBBY_H
