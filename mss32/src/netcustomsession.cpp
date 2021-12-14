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
#include "log.h"
#include "mempool.h"
#include "netcustomplayerclient.h"
#include "netcustomplayerserver.h"
#include "netcustomservice.h"
#include <atomic>
#include <chrono>
#include <fmt/format.h>
#include <thread>

namespace hooks {

extern std::atomic_bool hostAddressSet;

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
    logDebug("lobby.log", "CNetCustomSession getMaxClients called");
    return 4;
}

void __fastcall netCustomSessionGetPlayers(CNetCustomSession* thisptr,
                                           int /*%edx*/,
                                           game::LinkedList<game::IMqNetPlayerEnum*>* players)
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

    // TODO: we should already have a room, get server address from its custom data
    // TODO: check if client is not nullptr
    auto playerClient = createCustomPlayerClient(thisptr, netSystem, reception, clientName);
    thisptr->players.push_back((CNetCustomPlayerClient*)playerClient);
    *client = playerClient;
}

void __fastcall netCustomSessionCreateServer(CNetCustomSession* thisptr,
                                             int /*%edx*/,
                                             game::IMqNetPlayerServer** server,
                                             game::IMqNetSystem* netSystem,
                                             game::IMqNetReception* reception)
{
    logDebug("lobby.log", "CNetCustomSession createServer called");
    auto playerServer = createCustomPlayerServer(thisptr, netSystem, reception);
    thisptr->server = (CNetCustomPlayerServer*)playerServer;
    *server = playerServer;
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
