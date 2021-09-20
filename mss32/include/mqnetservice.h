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

#ifndef MQNETSERVICE_H
#define MQNETSERVICE_H

namespace game {

struct GUID;
struct IMqNetSession;
struct IMqNetSessEnum;
struct IMqNetServiceVftable;

struct IMqNetService
{
    IMqNetServiceVftable* vftable;
};

struct IMqNetServiceVftable
{
    using Destructor = void(__thiscall*)(IMqNetService* thisptr, char flags);
    Destructor destructor;

    using HasSessions = bool(__thiscall*)(IMqNetService* thisptr);
    HasSessions hasSessions;

    using GetSessions = void(__thiscall*)(IMqNetService* thisptr,
                                          int a1,
                                          const GUID* appGuid,
                                          const char* ipAddress,
                                          bool a5,
                                          bool a6);
    GetSessions getSessions;

    /**
     * Creates session with specified guid, name and password.
     * CNetDPlayService calls IDirectPlay4::Open with DPOPEN_CREATE here.
     */
    using CreateSession = void(__thiscall*)(IMqNetService* thisptr,
                                            IMqNetSession** netSession,
                                            const GUID* appGuid,
                                            const char* sessionName,
                                            const char* password);
    CreateSession createSession;

    /**
     * Joins session specified by netSessionEnum.
     * CNetDPlayService calls IDirectPlay4::Open with DPOPEN_JOIN here.
     */
    using JoinSession = void(__thiscall*)(IMqNetService* thisptr,
                                          IMqNetSession** netSession,
                                          IMqNetSessEnum* netSessionEnum,
                                          const char* password);
    JoinSession joinSession;
};

static_assert(sizeof(IMqNetServiceVftable) == 5 * sizeof(void*),
              "IMqNetService vftable must have exactly 5 methods");

} // namespace game

#endif // MQNETSERVICE_H
