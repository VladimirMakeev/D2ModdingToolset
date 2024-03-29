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

#include "netcustomsession.h"
#include "d2string.h"
#include "lobbyclient.h"
#include "log.h"
#include "mempool.h"
#include "mqnetsystem.h"
#include "netcustomplayerclient.h"
#include "netcustomplayerserver.h"
#include "netcustomservice.h"
#include <atomic>
#include <chrono>
#include <fmt/format.h>
#include <thread>

namespace hooks {

extern std::atomic_bool hostAddressSet;

bool CNetCustomSession::setMaxPlayers(int maxPlayers)
{
    if (maxPlayers < 1 || maxPlayers > 4) {
        return false;
    }

    // -1 because room already have moderator.
    const auto result{tryChangeRoomPublicSlots(maxPlayers - 1)};
    if (result) {
        this->maxPlayers = maxPlayers;
    }

    return result;
}

void __fastcall netCustomSessionDtor(CNetCustomSession* thisptr, int /*%edx*/, char flags)
{
    logDebug("lobby.log", "CNetCustomSession d-tor called");

    thisptr->name.~basic_string();
    thisptr->players.~vector();

    if (flags & 1) {
        logDebug("lobby.log", "CNetCustomSession d-tor frees memory");
        game::Memory::get().freeNonZero(thisptr);
    }
}

game::String* __fastcall netCustomSessionGetName(CNetCustomSession* thisptr,
                                                 int /*%edx*/,
                                                 game::String* sessionName)
{
    logDebug("lobby.log", "CNetCustomSession getName called");

    game::StringApi::get().initFromString(sessionName, thisptr->name.c_str());
    return sessionName;
}

int __fastcall netCustomSessionGetClientsCount(CNetCustomSession* thisptr, int /*%edx*/)
{
    logDebug("lobby.log", "CNetCustomSession getClientsCount called");
    return (int)thisptr->players.size();
}

int __fastcall netCustomSessionGetMaxClients(CNetCustomSession* thisptr, int /*%edx*/)
{
    logDebug("lobby.log", fmt::format("CNetCustomSession getMaxClients called. Max clients: {:d}",
                                      thisptr->maxPlayers));
    return thisptr->maxPlayers;
}

void __fastcall netCustomSessionGetPlayers(CNetCustomSession* thisptr,
                                           int /*%edx*/,
                                           game::List<game::IMqNetPlayerEnum*>* players)
{
    logDebug("lobby.log", "CNetCustomSession getPlayers called");
}

void __fastcall netCustomSessionCreateClient(CNetCustomSession* thisptr,
                                             int /*%edx*/,
                                             game::IMqNetPlayerClient** client,
                                             game::IMqNetSystem* netSystem,
                                             game::IMqNetReception* reception,
                                             const char* clientName)
{
    logDebug("lobby.log", fmt::format("CNetCustomSession createClient '{:s}' called", clientName));

    auto customClient = thisptr->players.front();
    // Finalize player setup here when we know all the settings
    auto& player = customClient->player;
    player.name = clientName;
    player.netSystem = netSystem;
    player.netReception = reception;

    *client = customClient;
}

void __fastcall netCustomSessionCreateServer(CNetCustomSession* thisptr,
                                             int /*%edx*/,
                                             game::IMqNetPlayerServer** server,
                                             game::IMqNetSystem* netSystem,
                                             game::IMqNetReception* reception)
{
    logDebug("lobby.log", "CNetCustomSession createServer called");

    *server = nullptr;
    if (!thisptr->server) {
        logDebug("lobby.log", "Player server is null !!!");
        return;
    }

    auto& serverPlayer = thisptr->server->player;
    serverPlayer.netSystem = netSystem;
    serverPlayer.netReception = reception;

    *server = thisptr->server;
}

static game::IMqNetSessionVftable netCustomSessionVftable{
    (game::IMqNetSessionVftable::Destructor)netCustomSessionDtor,
    (game::IMqNetSessionVftable::GetName)netCustomSessionGetName,
    (game::IMqNetSessionVftable::GetClientsCount)netCustomSessionGetClientsCount,
    (game::IMqNetSessionVftable::GetMaxClients)netCustomSessionGetMaxClients,
    (game::IMqNetSessionVftable::GetPlayers)netCustomSessionGetPlayers,
    (game::IMqNetSessionVftable::CreateClient)netCustomSessionCreateClient,
    (game::IMqNetSessionVftable::CreateServer)netCustomSessionCreateServer,
};

game::IMqNetSession* createCustomNetSession(CNetCustomService* service,
                                            const char* sessionName,
                                            bool host)
{
    auto session = (CNetCustomSession*)game::Memory::get().allocate(sizeof(CNetCustomSession));
    new (session) CNetCustomSession(service, sessionName, host);

    session->vftable = &netCustomSessionVftable;
    return session;
}

} // namespace hooks
