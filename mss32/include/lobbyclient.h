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

#ifndef LOBBYCLIENT_H
#define LOBBYCLIENT_H

#include <MessageIdentifiers.h>

namespace hooks {

extern const char* serverGuidColumnName;
extern const char* passwordColumnName;

enum ClientMessages
{
    ID_CHECK_FILES_INTEGRITY = ID_USER_PACKET_ENUM + 1,
    ID_FILES_INTEGRITY_RESULT,
};

/**
 * Tries to register new account using credentials provided.
 * @returns true if register request message was successfully send to the server.
 */
bool tryCreateAccount(const char* accountName,
                      const char* nickname,
                      const char* password,
                      const char* pwdRecoveryQuestion,
                      const char* pwdRecoveryAnswer);

/**
 * Tries to login user with provided credentials.
 * @returns true if login request message was successfully send to the server.
 */
bool tryLoginAccount(const char* accountName, const char* password);

/** Logouts currently logged user. */
void logoutAccount();

void setCurrentLobbyPlayer(const char* accountName);

/** Tries to create and enter a new room. */
bool tryCreateRoom(const char* roomName, const char* serverGuid, const char* password = nullptr);

/** Requests a list of rooms for specified account. */
bool trySearchRooms(const char* accountName = nullptr);

/** Tries to join existing room by its name. */
bool tryJoinRoom(const char* roomName);

/** Tries to change number of public slots in current room. */
bool tryChangeRoomPublicSlots(unsigned int publicSlots);

/** Tries to request files integrity check from the server. */
bool tryCheckFilesIntegrity(const char* hash);

} // namespace hooks

#endif // LOBBYCLIENT_H
