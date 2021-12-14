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

#include <string>
#include <vector>

namespace game {
struct CMenuBase;
struct CMenuPhase;
} // namespace game

namespace hooks {

struct CMenuCustomLobby;

struct RoomInfo
{
    std::string name;
    std::string hostName;
    int publicSlots;
    int remainingSlots;
};

game::CMenuBase* __stdcall createCustomLobbyMenu(game::CMenuPhase* menuPhase);

void customLobbyShowError(const char* message);

void customLobbyProcessLogin(CMenuCustomLobby* menu, const char* accountName);

void customLobbyProcessLogout(CMenuCustomLobby* menu);

void customLobbySetRoomsInfo(CMenuCustomLobby* menu, std::vector<RoomInfo>&& rooms);

void customLobbyProcessJoin(CMenuCustomLobby* menu, const char* roomName);

void customLobbyProcessJoinError(CMenuCustomLobby* menu, const char* message);

} // namespace hooks

#endif // MENUCUSTOMLOBBY_H
