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

#include "lobbycallbacks.h"
#include "lobbyclient.h"
#include "log.h"
#include "menucustomlobby.h"
#include "roomclientjoin.h"
#include <fmt/format.h>

namespace hooks {

void LoggingCallbacks::ExecuteDefaultResult(SLNet::Lobby2Message* msg)
{
    SLNet::RakString str;
    msg->DebugMsg(str);

    logDebug("lobbyCallbacks.log", str.C_String());
}

void UiUpdateCallbacks::MessageResult(SLNet::Client_Login* message)
{
    switch (message->resultCode) {
    case SLNet::L2RC_SUCCESS:
        customLobbyProcessLogin(menuLobby, message->userName.C_String());
        break;

    case SLNet::L2RC_Client_Login_HANDLE_NOT_IN_USE_OR_BAD_SECRET_KEY:
        customLobbyShowError("Wrong account name or password");
        break;

    case SLNet::L2RC_Client_Login_BANNED:
        customLobbyShowError("Banned from server");
        break;

    default: {
        SLNet::RakString str;
        message->DebugMsg(str);
        customLobbyShowError(str.C_String());
        break;
    }
    }
}

void UiUpdateCallbacks::MessageResult(SLNet::Client_Logoff* message)
{
    if (message->resultCode == SLNet::L2RC_SUCCESS) {
        customLobbyProcessLogout(menuLobby);
        return;
    }

    SLNet::RakString str;
    message->DebugMsg(str);
    customLobbyShowError(str.C_String());
}

void RoomsListCallbacks::JoinByFilter_Callback(const SLNet::SystemAddress&,
                                               SLNet::JoinByFilter_Func* callResult)
{
    if (callResult->resultCode == SLNet::REC_SUCCESS) {
        auto& room = callResult->joinedRoomResult.roomDescriptor;
        auto roomName = room.GetProperty(DefaultRoomColumns::TC_ROOM_NAME)->c;

        logDebug("roomsCallbacks.log",
                 fmt::format("{:s} joined room",
                             callResult->joinedRoomResult.joiningMemberName.C_String()));

        const char* guidString{nullptr};

        auto& table = room.roomProperties;
        auto index = table.ColumnIndex(serverGuidColumnName);
        if (index != std::numeric_limits<unsigned int>::max()) {
            auto row = table.GetRowByIndex(0, nullptr);
            if (row) {
                guidString = row->cells[index]->c;
            }
        }

        SLNet::RakNetGUID serverGuid{};
        if (!serverGuid.FromString(guidString)) {
            customLobbyProcessJoinError(menuLobby, "Could not get player server GUID");
            return;
        }

        customLobbyProcessJoin(menuLobby, roomName, serverGuid);
        return;
    }

    auto error = SLNet::RoomsErrorCodeDescription::ToEnglish(callResult->resultCode);
    customLobbyProcessJoinError(menuLobby, error);
}

void RoomsListCallbacks::SearchByFilter_Callback(const SLNet::SystemAddress&,
                                                 SLNet::SearchByFilter_Func* callResult)
{
    if (callResult->resultCode != SLNet::REC_SUCCESS) {
        customLobbyShowError(SLNet::RoomsErrorCodeDescription::ToEnglish(callResult->resultCode));
        return;
    }

    auto& rooms = callResult->roomsOutput;
    const auto total{rooms.Size()};

    std::vector<RoomInfo> roomsInfo;
    roomsInfo.reserve(total);

    for (std::uint32_t i = 0; i < total; ++i) {
        auto room = rooms[i];

        auto name = room->GetProperty(DefaultRoomColumns::TC_ROOM_NAME)->c;
        const char* hostName{nullptr};

        auto& members = room->roomMemberList;
        for (std::uint32_t j = 0; members.Size(); ++j) {
            auto& member = members[j];

            if (member.roomMemberMode == RMM_MODERATOR) {
                hostName = member.name.C_String();
                break;
            }
        }

        auto totalSlots = (int)room->GetProperty(DefaultRoomColumns::TC_TOTAL_PUBLIC_SLOTS)->i;
        auto usedSlots = (int)room->GetProperty(DefaultRoomColumns::TC_USED_PUBLIC_SLOTS)->i;

        const char* password{nullptr};

        auto& table = room->roomProperties;
        auto index = table.ColumnIndex(passwordColumnName);
        if (index != std::numeric_limits<unsigned int>::max()) {
            auto row = table.GetRowByIndex(0, nullptr);
            if (row) {
                password = row->cells[index]->c;
            }
        }

        // Add 1 to used and total slots because they are not counting room moderator
        roomsInfo.push_back(RoomInfo{std::string(name),
                                     std::string{hostName ? hostName : "Unknown"},
                                     password ? std::string{password} : std::string{},
                                     totalSlots + 1, usedSlots + 1});
    }

    customLobbySetRoomsInfo(menuLobby, std::move(roomsInfo));
}

} // namespace hooks
