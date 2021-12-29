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

#include "roomscallback.h"
#include "log.h"
#include "netcustomplayer.h"
#include "netcustomservice.h"
#include <RoomsErrorCodes.h>
#include <atomic>
#include <fmt/format.h>

namespace hooks {

std::atomic_bool hostAddressSet{false};

SLNet::SystemAddress lobbyAddressToServerPlayer(const SLNet::SystemAddress& lobbyAddress)
{
    auto addressString{lobbyAddress.ToString(false)};
    auto serverAddress{SLNet::SystemAddress(addressString, CNetCustomPlayer::serverPort)};

    logDebug("lobby.log", fmt::format("lobbyAddress: '{:s}', serverAddress '{:s}'", addressString,
                                      serverAddress.ToString()));
    return serverAddress;
}

void RoomsLoggingCallback::CreateRoom_Callback(const SLNet::SystemAddress& senderAddress,
                                               SLNet::CreateRoom_Func* callResult)
{
    if (callResult->resultCode != SLNet::REC_SUCCESS) {
        logDebug("roomsCallbacks.log",
                 fmt::format("Could not create a room. Reason: {:s}",
                             SLNet::RoomsErrorCodeDescription::ToEnglish(callResult->resultCode)));
        return;
    }

    auto cell{callResult->roomDescriptor.GetProperty(DefaultRoomColumns::TC_ROOM_NAME)};
    if (!cell) {
        logDebug("roomsCallbacks.log", fmt::format("Count not get room name cell"));
        return;
    }

    auto name{cell->c};
    logDebug("roomsCallbacks.log",
             fmt::format("Player {:s} created room {:s}", callResult->userName.C_String(), name));

    auto service = getNetService();
    if (service) {
        logDebug("roomsCallbacks.log", "I am the room moderator, set my own address as a host.");
        // Get our own system address, convert it to server-player address
        service->roomOwnerAddress = lobbyAddressToServerPlayer(
            service->lobbyPeer.peer->GetMyBoundAddress());
        hostAddressSet = true;
    }
}

void RoomsLoggingCallback::EnterRoom_Callback(const SLNet::SystemAddress& senderAddress,
                                              SLNet::EnterRoom_Func* callResult)
{
    if (callResult->resultCode != SLNet::REC_SUCCESS) {
        logDebug("roomsCallbacks.log",
                 fmt::format("Could not join room. Reason: {:s}",
                             SLNet::RoomsErrorCodeDescription::ToEnglish(callResult->resultCode)));
        return;
    }

    auto& room{callResult->joinedRoomResult.roomDescriptor};
    auto cell{room.GetProperty(DefaultRoomColumns::TC_ROOM_NAME)};
    if (!cell) {
        logDebug("roomsCallbacks.log", fmt::format("Count not get room name cell"));
        return;
    }

    auto name{cell->c};
    logDebug("roomsCallbacks.log",
             fmt::format("Player {:s} {:s} room {:s}", callResult->userName.C_String(),
                         (callResult->createdRoom ? "tried to join and created new" : "joined the"),
                         name));

    auto& members = room.roomMemberList;
    logDebug("roomsCallbacks.log", fmt::format("Room has {:d} members", members.Size()));

    // remember room creator address

    for (unsigned int i = 0; i < members.Size(); ++i) {
        auto& member = members[i];
        logDebug("roomsCallbacks.log",
                 fmt::format("Check member {:d}, mode {:d}", i, (int)member.roomMemberMode));
        if (member.roomMemberMode == RoomMemberMode::RMM_MODERATOR) {
            auto service = getNetService();
            if (service) {
                logDebug("roomsCallbacks.log", "Found room moderator, assume its a game host.");
                // Get member system address, convert to server-player address
                service->roomOwnerAddress = lobbyAddressToServerPlayer(member.systemAddress);
                hostAddressSet = true;
            }
        }
    }
}

void RoomsLoggingCallback::LeaveRoom_Callback(const SLNet::SystemAddress& senderAddress,
                                              SLNet::LeaveRoom_Func* callResult)
{
    logDebug("roomsCallbacks.log",
             fmt::format("Player {:s} left the room. Result {:s}", callResult->userName.C_String(),
                         SLNet::RoomsErrorCodeDescription::ToEnglish(callResult->resultCode)));
}

void RoomsLoggingCallback::RoomMemberLeftRoom_Callback(
    const SLNet::SystemAddress& senderAddress,
    SLNet::RoomMemberLeftRoom_Notification* notification)
{
    logDebug("roomsCallbacks.log",
             fmt::format("Notification to {:s}. {:s} has left the room",
                         notification->recipient.C_String(), notification->roomMember.C_String()));
}

void RoomsLoggingCallback::RoomMemberJoinedRoom_Callback(
    const SLNet::SystemAddress& senderAddress,
    SLNet::RoomMemberJoinedRoom_Notification* notification)
{
    logDebug("roomsCallbacks.log",
             fmt::format("Notification to {:s}. {:s} has joined the room",
                         notification->recipient.C_String(),
                         notification->joinedRoomResult->joiningMemberName.C_String()));
}

} // namespace hooks
