/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef MIDSERVER_H
#define MIDSERVER_H

#include "mqnetsystem.h"
#include "mqthread.h"
#include <cstddef>
#include <cstdint>

namespace game {

struct IMqNetSession;
struct IMqNetPlayerServer;
struct CMidServerBuilderFull;
struct CMidServerLogic;
struct NetMsgCallbacks;
struct NetMsgEntryData;
struct CNetMsg;

struct CMidServerData
{
    IMqNetSession* netSession;
    IMqNetPlayerServer* netPlayerServer;
    NetMsgCallbacks** netCallbacks;
    NetMsgEntryData** netMsgEntryData;
    std::uint32_t serverMessageId;
    int unknown2;
    CMidServerBuilderFull* serverBuilder;
    CMidServerLogic* serverLogic;
};

assert_size(CMidServerData, 32);

struct CMidServer
    : public CMqThread
    , public IMqNetSystem
{
    CMidServerData* data;
};

assert_size(CMidServer, 16);
assert_offset(CMidServer, CMidServer::CMqThread::vftable, 0);
assert_offset(CMidServer, CMidServer::IMqNetSystem::vftable, 8);
assert_offset(CMidServer, data, 12);

namespace CMidServerApi {

struct Api
{
    using SendNetMsg = bool(__thiscall*)(const CMidServer* thisptr,
                                         const CNetMsg* netMsg,
                                         std::uint32_t playerNetId);
    SendNetMsg sendNetMsg;
};

Api& get();

} // namespace CMidServerApi

} // namespace game

#endif // MIDSERVER_H
