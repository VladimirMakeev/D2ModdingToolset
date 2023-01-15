/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef NETMSG_H
#define NETMSG_H

#include "d2assert.h"
#include <cstdint>

namespace game {

struct CNetMsgVftable;
struct CMqStream;

/**
 * Network messages common header part.
 * Each ingame message that is being sent or received over network starts with it.
 * CNetMsg fills and checks header in its serialize method.
 */
struct NetMessageHeader
{
    /** Equals to netMessageNormalType for game specific messages. */
    std::uint32_t messageType;
    /** Length of message in bytes, including header. */
    std::uint32_t length;
    /**
     * Raw name of CNetMsg or its derived class.
     * Obtained as @code{.cpp}typeid(msg).raw_name();@endcode which is Windows specific.
     * @see https://docs.microsoft.com/en-us/cpp/cpp/type-info-class for additional info.
     */
    char messageClassName[36];
};

assert_size(NetMessageHeader, 44);

/** Game specific messages treated as normal in DirectPlay terms. */
static constexpr std::uint32_t netMessageNormalType{0xffff};

/** Maximum allowed net message length in bytes. */
static constexpr std::uint32_t netMessageMaxLength{0x80000};

/** Used for sending messages between players in single, DPlay and Lobby sessions. */
static constexpr std::uint32_t broadcastNetPlayerId{0};
static constexpr std::uint32_t serverNetPlayerId{1};
static constexpr std::uint32_t singleNetPlayerId{0xffffff};

template <typename T = CNetMsgVftable>
struct CNetMsgT
{
    const T* vftable;
};

struct CNetMsg : public CNetMsgT<>
{ };

assert_size(CNetMsg, 4);

struct CNetMsgVftable
{
    using Destructor = void(__thiscall*)(CNetMsg* thisptr, char flags);
    Destructor destructor;

    using Serialize = void(__thiscall*)(CNetMsg* thisptr, CMqStream* stream);
    Serialize serialize;
};

assert_vftable_size(CNetMsgVftable, 2);

namespace CNetMsgApi {

struct Api
{
    using Destructor = void(__thiscall*)(CNetMsg* thisptr);
    Destructor destructor;
};

Api& get();

CNetMsgVftable* vftable();

} // namespace CNetMsgApi

} // namespace game

#endif // NETMSG_H
