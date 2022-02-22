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

#include "netcustomplayer.h"
#include "d2string.h"
#include "log.h"
#include "mempool.h"
#include "mqnetplayer.h"
#include "mqnetreception.h"
#include "mqnetsystem.h"
#include "netcustomsession.h"
#include "netmsg.h"
#include <fmt/format.h>
#include <mutex>
#include <string>

namespace hooks {

void playerLog(const std::string& message)
{
    static std::mutex logMutex;
    std::lock_guard lock(logMutex);

    logDebug("lobby.log", message);
}

static void __fastcall netCustomPlayerDtor(CNetCustomPlayer* thisptr, int /*%edx*/, char flags)
{
    playerLog("CNetCustomPlayer d-tor called");

    thisptr->~CNetCustomPlayer();

    if (flags & 1) {
        playerLog("CNetCustomPlayer free memory");
        game::Memory::get().freeNonZero(thisptr);
    }
}

static game::String* __fastcall netCustomPlayerGetName(CNetCustomPlayer* thisptr,
                                                       int /*%edx*/,
                                                       game::String* string)
{
    playerLog(fmt::format("CNetCustomPlayer {:s} getName", thisptr->name));
    game::StringApi::get().initFromString(string, thisptr->name.c_str());
    return string;
}

static int __fastcall netCustomPlayerGetNetId(CNetCustomPlayer* thisptr, int /*%edx*/)
{
    playerLog(fmt::format("CNetCustomPlayer {:s} getNetId 0x{:x}", thisptr->name, thisptr->netId));
    return static_cast<int>(thisptr->netId);
}

static game::IMqNetSession* __fastcall netCustomPlayerGetSession(CNetCustomPlayer* thisptr,
                                                                 int /*%edx*/)
{
    playerLog(fmt::format("CNetCustomPlayer {:s} getSession", thisptr->name));
    return thisptr->session;
}

static int __fastcall netCustomPlayerGetMessageCount(CNetCustomPlayer* thisptr, int /*%edx*/)
{
    playerLog(fmt::format("CNetCustomPlayer {:s} getMessageCount", thisptr->name));
    return 1;
}

static bool __fastcall netCustomPlayerSendMessage(CNetCustomPlayer* thisptr,
                                                  int /*%edx*/,
                                                  int idTo,
                                                  const game::NetMessageHeader* message)
{
    playerLog(fmt::format("CNetCustomPlayer {:s} sendMessage '{:s}' to {:d}", thisptr->name,
                          message->messageClassName, idTo));
    return true;
}

static int __fastcall netCustomPlayerReceiveMessage(CNetCustomPlayer* thisptr,
                                                    int /*%edx*/,
                                                    int* idFrom,
                                                    game::NetMessageHeader* buffer)
{
    playerLog(fmt::format("CNetCustomPlayer {:s} receiveMessage", thisptr->name));
    return 0;
}

static void __fastcall netCustomPlayerSetNetSystem(CNetCustomPlayer* thisptr,
                                                   int /*%edx*/,
                                                   game::IMqNetSystem* netSystem)
{
    playerLog(fmt::format("CNetCustomPlayer {:s} setNetSystem", thisptr->name));
    if (thisptr->netSystem != netSystem) {
        if (thisptr->netSystem) {
            thisptr->netSystem->vftable->destructor(thisptr->netSystem, 1);
        }
        thisptr->netSystem = netSystem;
    }
}

static int __fastcall netCustomPlayerMethod8(CNetCustomPlayer* thisptr, int /*%edx*/, int a2)
{
    playerLog(fmt::format("CNetCustomPlayer {:s} method8", thisptr->name));
    return a2;
}

static game::IMqNetPlayerVftable netCustomPlayerVftable{
    (game::IMqNetPlayerVftable::Destructor)netCustomPlayerDtor,
    (game::IMqNetPlayerVftable::GetName)netCustomPlayerGetName,
    (game::IMqNetPlayerVftable::GetNetId)netCustomPlayerGetNetId,
    (game::IMqNetPlayerVftable::GetSession)netCustomPlayerGetSession,
    (game::IMqNetPlayerVftable::GetMessageCount)netCustomPlayerGetMessageCount,
    (game::IMqNetPlayerVftable::SendNetMessage)netCustomPlayerSendMessage,
    (game::IMqNetPlayerVftable::ReceiveMessage)netCustomPlayerReceiveMessage,
    (game::IMqNetPlayerVftable::SetNetSystem)netCustomPlayerSetNetSystem,
    (game::IMqNetPlayerVftable::Method8)netCustomPlayerMethod8,
};

CNetCustomPlayer::CNetCustomPlayer(CNetCustomSession* session,
                                   game::IMqNetSystem* netSystem,
                                   game::IMqNetReception* netReception,
                                   const char* name,
                                   NetworkPeer::PeerPtr&& peer,
                                   std::uint32_t netId)
    : name{name}
    , netPeer{std::move(peer)}
    , session{session}
    , netSystem{netSystem}
    , netReception{netReception}
    , netId{netId}
{
    vftable = &netCustomPlayerVftable;
}

CNetCustomPlayer::~CNetCustomPlayer()
{
    if (netSystem) {
        playerLog("CNetCustomPlayer delete netSystem");
        netSystem->vftable->destructor(netSystem, 1);
    }

    if (netReception) {
        playerLog("CNetCustomPlayer delete netReception");
        netReception->vftable->destructor(netReception, 1);
    }
}

} // namespace hooks
