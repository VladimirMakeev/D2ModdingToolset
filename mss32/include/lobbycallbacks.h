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

#ifndef LOBBYCALLBACKS_H
#define LOBBYCALLBACKS_H

#include "menucustomlobby.h"

#ifdef __INTEGRATE_LOBBY2_WITH_ROOMS_PLUGIN
#include <RoomsPlugin.h>
#endif

#include <Lobby2Message.h>

namespace hooks {

class LoggingCallbacks : public SLNet::Lobby2Callbacks
{
public:
    LoggingCallbacks() = default;
    ~LoggingCallbacks() override = default;

    void ExecuteDefaultResult(SLNet::Lobby2Message* msg) override;
};

class UiUpdateCallbacks : public SLNet::Lobby2Callbacks
{
public:
    UiUpdateCallbacks(CMenuCustomLobby* menuLobby)
        : menuLobby{menuLobby}
    { }

    ~UiUpdateCallbacks() override = default;

    void MessageResult(SLNet::Client_Login* message) override;
    void MessageResult(SLNet::Client_Logoff* message) override;

private:
    CMenuCustomLobby* menuLobby;
};

class RoomsListCallbacks : public SLNet::RoomsCallback
{
public:
    RoomsListCallbacks(CMenuCustomLobby* menuLobby)
        : menuLobby{menuLobby}
    { }

    ~RoomsListCallbacks() override = default;

    void JoinByFilter_Callback(const SLNet::SystemAddress& senderAddress,
                               SLNet::JoinByFilter_Func* callResult) override;

    void SearchByFilter_Callback(const SLNet::SystemAddress& senderAddress,
                                 SLNet::SearchByFilter_Func* callResult) override;

private:
    CMenuCustomLobby* menuLobby;
};

} // namespace hooks

#endif // LOBBYCALLBACKS_H
