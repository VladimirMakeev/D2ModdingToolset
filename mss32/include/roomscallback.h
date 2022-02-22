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

#ifndef ROOMSCALLBACK_H
#define ROOMSCALLBACK_H

#ifdef __INTEGRATE_LOBBY2_WITH_ROOMS_PLUGIN
#include <RoomsPlugin.h>
#endif

namespace hooks {

class RoomsLoggingCallback : public SLNet::RoomsCallback
{
public:
    RoomsLoggingCallback() = default;
    ~RoomsLoggingCallback() override = default;

    void CreateRoom_Callback(const SLNet::SystemAddress& senderAddress,
                             SLNet::CreateRoom_Func* callResult) override;

    void EnterRoom_Callback(const SLNet::SystemAddress& senderAddress,
                            SLNet::EnterRoom_Func* callResult) override;

    void LeaveRoom_Callback(const SLNet::SystemAddress& senderAddress,
                            SLNet::LeaveRoom_Func* callResult) override;

    void RoomMemberLeftRoom_Callback(const SLNet::SystemAddress& senderAddress,
                                     SLNet::RoomMemberLeftRoom_Notification* notification) override;

    void RoomMemberJoinedRoom_Callback(
        const SLNet::SystemAddress& senderAddress,
        SLNet::RoomMemberJoinedRoom_Notification* notification) override;
};

} // namespace hooks

#endif // ROOMSCALLBACK_H
