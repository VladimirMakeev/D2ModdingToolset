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

#include "lobbyclient.h"
#include "log.h"
#include "netcustomservice.h"
#include <fmt/format.h>

namespace hooks {

// Hardcoded in client and server
#if 0
static const char titleName[]{"Test Title Name"};
static const char titleSecretKey[]{"Test secret key"};
#else
static const char titleName[]{"Disciples2 Motlin"};
static const char titleSecretKey[]{"Disciples2 Key"};
#endif

const char* serverGuidColumnName{"ServerGuid"};
const char* passwordColumnName{"Password"};

bool tryCreateAccount(const char* accountName,
                      const char* nickname,
                      const char* password,
                      const char* pwdRecoveryQuestion,
                      const char* pwdRecoveryAnswer)
{
    if (!accountName) {
        logDebug("lobby.log", "Empty account name");
        return false;
    }

    if (!nickname) {
        logDebug("lobby.log", "Empty nick name");
        return false;
    }

    if (!password) {
        logDebug("lobby.log", "Empty password");
        return false;
    }

    if (!pwdRecoveryQuestion) {
        logDebug("lobby.log", "Empty pwd recv question");
        return false;
    }

    if (!pwdRecoveryAnswer) {
        logDebug("lobby.log", "Empty pwd recv answer");
        return false;
    }

    auto netService{getNetService()};
    if (!netService) {
        logDebug("lobby.log", "No net service in midgard");
        return false;
    }

    auto msg{netService->lobbyMsgFactory.Alloc(SLNet::L2MID_Client_RegisterAccount)};
    auto account{static_cast<SLNet::Client_RegisterAccount*>(msg)};
    if (!account) {
        logDebug("lobby.log", "Can not allocate account message");
        return false;
    }

    account->userName = accountName;
    account->titleName = titleName;

    auto& params = account->createAccountParameters;
    params.firstName = nickname;
    params.lastName = params.firstName;
    params.password = password;
    params.passwordRecoveryQuestion = pwdRecoveryQuestion;
    params.passwordRecoveryAnswer = pwdRecoveryAnswer;

    const auto result{account->PrevalidateInput()};
    if (!result) {
        logDebug("lobby.log", "Wrong account registration data");
    } else {
        netService->lobbyClient.SendMsg(account);
        logDebug("lobby.log", "Account registration sent");
    }

    netService->lobbyMsgFactory.Dealloc(account);
    return result;
}

bool tryLoginAccount(const char* accountName, const char* password)
{
    if (!accountName) {
        return false;
    }

    if (!password) {
        return false;
    }

    auto netService{getNetService()};
    if (!netService) {
        logDebug("lobby.log", "No net service in midgard");
        return false;
    }

    auto msg{netService->lobbyMsgFactory.Alloc(SLNet::L2MID_Client_Login)};
    auto login{static_cast<SLNet::Client_Login*>(msg)};
    if (!login) {
        logDebug("lobby.log", "Can not allocate login message");
        return false;
    }

    login->userName = accountName;
    login->userPassword = password;

    login->titleName = titleName;
    login->titleSecretKey = titleSecretKey;

    const auto result{login->PrevalidateInput()};
    if (!result) {
        logDebug("lobby.log", "Wrong account data");
    } else {
        netService->lobbyClient.SendMsg(login);
        logDebug("lobby.log", "Trying to login");
    }

    netService->lobbyMsgFactory.Dealloc(login);
    return result;
}

void logoutAccount()
{
    auto netService{getNetService()};
    if (!netService) {
        logDebug("lobby.log", "No net service in midgard");
        return;
    }

    auto msg{netService->lobbyMsgFactory.Alloc(SLNet::L2MID_Client_Logoff)};
    auto logoff{static_cast<SLNet::Client_Logoff*>(msg)};
    if (!logoff) {
        logDebug("lobby.log", "Can not allocate logoff message");
        return;
    }

    netService->lobbyClient.SendMsg(logoff);
    netService->lobbyMsgFactory.Dealloc(logoff);
}

void setCurrentLobbyPlayer(const char* accountName)
{
    auto netService{getNetService()};
    if (!netService) {
        return;
    }

    if (accountName) {
        logDebug("lobby.log", fmt::format("Set current logged accout to {:s}", accountName));
        netService->loggedAccount = accountName;
    } else {
        logDebug("lobby.log", "Forget current logged accout");
        netService->loggedAccount.clear();
    }
}

bool tryCreateRoom(const char* roomName, const char* serverGuid, const char* password)
{
    if (!roomName) {
        logDebug("lobby.log", "Could not create a room: no room name provided");
        return false;
    }

    if (!serverGuid) {
        logDebug("lobby.log", "Could not create a room: empty server guid");
        return false;
    }

    auto netService{getNetService()};
    if (!netService) {
        logDebug("lobby.log", "No net service in midgard");
        return false;
    }

    SLNet::CreateRoom_Func room{};
    room.gameIdentifier = titleName;
    room.userName = netService->loggedAccount.c_str();
    room.resultCode = SLNet::REC_SUCCESS;

    auto& params = room.networkedRoomCreationParameters;
    params.roomName = roomName;
    params.slots.publicSlots = 1;
    params.slots.reservedSlots = 0;
    params.slots.spectatorSlots = 0;

    logDebug("lobby.log",
             fmt::format("Account {:s} is trying to create and enter a room "
                         "with serverGuid {:s}, password {:s}",
                         room.userName.C_String(), serverGuid, (password ? password : "")));

    auto& properties = room.initialRoomProperties;

    const auto guidColumn{
        properties.AddColumn(serverGuidColumnName, DataStructures::Table::STRING)};

    constexpr auto invalidColumn{std::numeric_limits<unsigned int>::max()};
    if (guidColumn == invalidColumn) {
        logDebug("lobby.log",
                 fmt::format("Could not add server guid column to room properties table"));
        return false;
    }

    unsigned int passwordColumn{invalidColumn};
    if (password) {
        passwordColumn = properties.AddColumn(passwordColumnName, DataStructures::Table::STRING);
        if (passwordColumn == invalidColumn) {
            logDebug("lobby.log",
                     fmt::format("Could not add password column to room properties table"));
            return false;
        }
    }

    auto row = properties.AddRow(0);
    if (!row) {
        logDebug("lobby.log", "Could not add row to room properties table");
        return false;
    }

    row->UpdateCell(guidColumn, serverGuid);
    if (password) {
        row->UpdateCell(passwordColumn, password);
    }

    logDebug("lobby.log", fmt::format("Account {:s} is trying to create and enter a room {:s}",
                                      room.userName.C_String(), params.roomName.C_String()));
    netService->roomsClient.ExecuteFunc(&room);
    return true;
}

bool trySearchRooms(const char* accountName)
{
    auto netService{getNetService()};
    if (!netService) {
        logDebug("lobby.log", "No net service in midgard");
        return false;
    }

    SLNet::SearchByFilter_Func search;
    search.gameIdentifier = titleName;
    search.userName = accountName ? accountName : netService->loggedAccount.c_str();

    logDebug("lobby.log",
             fmt::format("Account {:s} is trying to search rooms", search.userName.C_String()));

    netService->roomsClient.ExecuteFunc(&search);
    return true;
}

bool tryJoinRoom(const char* roomName)
{
    if (!roomName) {
        logDebug("lobby.log", "Could not join room without a name");
        return false;
    }

    auto netService{getNetService()};
    if (!netService) {
        logDebug("lobby.log", "No net service in midgard");
        return false;
    }

    SLNet::JoinByFilter_Func join;
    join.gameIdentifier = titleName;
    join.userName = netService->loggedAccount.c_str();
    join.query.AddQuery_STRING(DefaultRoomColumns::GetColumnName(DefaultRoomColumns::TC_ROOM_NAME),
                               roomName);
    join.roomMemberMode = RMM_PUBLIC;

    logDebug("lobby.log", fmt::format("Account {:s} is trying to join room {:s}",
                                      join.userName.C_String(), roomName));

    netService->roomsClient.ExecuteFunc(&join);
    return true;
}

bool tryChangeRoomPublicSlots(unsigned int publicSlots)
{
    if (publicSlots < 1) {
        logDebug("lobby.log", "Could not set number of room public slots lesser than 1");
        return false;
    }

    auto netService{getNetService()};
    if (!netService) {
        logDebug("lobby.log", "No net service in midgard");
        return false;
    }

    SLNet::ChangeSlotCounts_Func slotCounts;
    slotCounts.userName = netService->loggedAccount.c_str();
    slotCounts.slots.publicSlots = publicSlots;

    logDebug("lobby.log", fmt::format("Account {:s} is trying to change room public slots to {:d}",
                                      slotCounts.userName.C_String(), publicSlots));

    netService->roomsClient.ExecuteFunc(&slotCounts);
    return true;
}

bool tryCheckFilesIntegrity(const char* hash)
{
    if (!hash || !std::strlen(hash)) {
        logDebug("lobby.log", "Could not check client integrity with empty hash");
        return false;
    }

    auto netService{getNetService()};
    if (!netService) {
        logDebug("lobby.log", "No net service in midgard");
        return false;
    }

    SLNet::BitStream stream;
    stream.Write(static_cast<SLNet::MessageID>(ID_CHECK_FILES_INTEGRITY));
    stream.Write(hash);

    const auto serverAddress{netService->lobbyClient.GetServerAddress()};
    const auto result{netService->lobbyPeer.peer->Send(&stream, PacketPriority::HIGH_PRIORITY,
                                                       PacketReliability::RELIABLE_ORDERED, 0,
                                                       serverAddress, false)};

    return result != 0;
}

} // namespace hooks
